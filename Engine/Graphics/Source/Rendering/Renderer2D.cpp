#include "Luma/Rendering/Renderer2D.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/RenderPipeline.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Rendering/BindingGroup.h"
#include "Luma/Rendering/Device.h"
#include "Luma/Rendering/Texture.h"
#include "Luma/Containers/Array.h"
#include "Luma/Containers/StringConversion.h"
#include "Luma/Math/Matrix2.h"
#include "Luma/Math/Matrix3.h"
#include "Luma/Math/Matrix4.h"
#include "Luma/Math/MatrixTransform.hxx"
#include "Luma/Math/MatrixProjection.hxx"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector4.h"
#include "Luma/Math/Color.h"
#include "Luma/Rendering/Filter.h"
#include "Luma/Rendering/Sampler.h"
#include "Luma/Runtime/Assertion.h"
#include "Luma/Runtime/Path.h"
#include "Luma/BinaryData/RobotoFont.h"
#include "Luma/Memory/Memory.h"
#include "Luma/Rendering/Queue.h"
#include "Luma/Rendering/RenderPassDesc.h"

namespace Luma
{
    using namespace RHI;

    FRenderer2D::FRenderer2D(Ref<Device> device, uint32_t width, uint32_t height)
        : m_Device(device)
    {
        m_DefaultFont = Ref<Font>::create();
        m_DefaultFont->loadAndGenerate(robotoFontData, FontAtlasType::MSDF, {CharacterSet::ascii()}, m_Device);
        setFont(m_DefaultFont);

        FString vertexPath = FPath::getEngineShaderPath("Renderer2D.slang.vert.spv");
        FString fragmentPath = FPath::getEngineShaderPath("Renderer2D.slang.frag.spv");
        m_Shader = m_Device->createShader(vertexPath, fragmentPath);

        VertexInputLayout vertexLayout;
        vertexLayout.addInputBinding(0, VertexInputRate::Vertex);
        vertexLayout.addInputAttribute({"POSITION", ShaderDataType::Float2, 0});
        vertexLayout.addInputAttribute({"TEXCOORD", ShaderDataType::Float2, 0});
        vertexLayout.addInputAttribute({"COLOR", ShaderDataType::Float4, 0});
        vertexLayout.addInputAttribute({"MODE", ShaderDataType::UInt, 0});
        vertexLayout.addInputAttribute({"TEXID", ShaderDataType::UInt, 0});

        RenderPipelineDesc rpDesc;
        rpDesc.shaderProgram = m_Shader;
        rpDesc.rasterization.cullMode = CullMode::None;
        rpDesc.colorTargetCount = 1;
        rpDesc.colorBlend[0] = ColorBlendState::alphaBlend();
        rpDesc.colorFormats[0] = Format::RGBA8_SRGB;
        rpDesc.inputLayout = vertexLayout;
        m_Pipeline = m_Device->createRenderPipeline(rpDesc);

        BufferDesc vbDesc;
        vbDesc.alwaysMapped = true;
        vbDesc.usage = BufferUsage::VertexBuffer;
        vbDesc.size = MAX_QUAD * 4 * sizeof(QuadVertex);
        m_VertexBuffer = m_Device->createBuffer(vbDesc);

        BufferDesc ibDesc;
        ibDesc.alwaysMapped = true;
        ibDesc.usage = BufferUsage::IndexBuffer;
        ibDesc.size = MAX_QUAD * 6 * sizeof(uint32_t);
        m_IndexBuffer = m_Device->createBuffer(ibDesc);

        SamplerDesc samplerDesc = SamplerDesc();
        samplerDesc.magFilter = Filter::Linear;
        samplerDesc.magFilter = Filter::Linear;
        m_Sampler = m_Device->getOrCreateSampler(samplerDesc);

        samplerDesc.magFilter = Filter::Nearest;
        samplerDesc.minFilter = Filter::Nearest;
        m_SpriteSampler = m_Device->getOrCreateSampler(samplerDesc);

        m_BindingGroup = m_Shader->createBindingGroup(0);
        m_BindingGroup->bindSampler(0, m_SpriteSampler);
        m_BindingGroup->bindSampler(1, m_Sampler);

        m_Fence = m_Device->createFence();



        TextureDesc textureDesc{};
        textureDesc.width = width;
        textureDesc.height = height;
        textureDesc.depth = 1;
        textureDesc.format = Format::RGBA8_SRGB;
        textureDesc.arrayCount = 1;
        textureDesc.sampleCount = 1;
        textureDesc.mipCount = 1;
        textureDesc.usageFlags = TextureUsage::ColorTarget | TextureUsage::Sampled;

        m_RenderTexture = m_Device->createTexture(textureDesc);
    }

    void FRenderer2D::destroy()
    {
        m_Device->waitIdle();
        m_DefaultFont = nullptr;
        m_Font = nullptr;
        m_BindingGroup = nullptr;
        m_Shader = nullptr;
        m_Pipeline = nullptr;
        m_VertexBuffer = nullptr;
        m_IndexBuffer = nullptr;
        m_Fence = nullptr;
        m_Device = nullptr;
        m_QuadVertices.free();
        m_QuadIndices.free();
    }


    void FRenderer2D::begin()
    {
        LUMA_ASSERT(!m_BeginDrawing, "begin/end mismatch");
        m_BeginDrawing = true;
        m_ReadyToRender = false;
        m_QuadVertices.clear();
        m_QuadIndices.clear();
        m_Textures.clear();
    }

    void FRenderer2D::end()
    {
        LUMA_ASSERT(m_BeginDrawing, "begin/end mismatch");

        void* vertexMapped = m_VertexBuffer->map();
        Memory::memcpy(vertexMapped, m_QuadVertices.data(), m_QuadVertices.size());
        m_VertexBuffer->unmap(vertexMapped);

        void* indexMapped = m_IndexBuffer->map();
        Memory::memcpy(indexMapped, m_QuadIndices.data(), m_QuadIndices.size());
        m_IndexBuffer->unmap(indexMapped);

        m_BindingGroup->bindTextures(2, m_Textures, TextureBindingType::Sampled);
        m_BeginDrawing = false;
        m_ReadyToRender = true;
    }

    Ref<Texture> FRenderer2D::render(const Camera& camera)
    {
        LUMA_ASSERT(m_ReadyToRender, "not ready to render yet!!");

        const FMatrix4f& projection = camera.getProjectionMatrix();

        Queue* renderQueue = m_Device->getRenderQueue();
        Ref<CommandBuffer> cmdBuffer = m_Device->createCommandBuffer(renderQueue);

        if (cmdBuffer->begin())
        {
            RenderPassTarget renderTarget;
            renderTarget.type = RenderPassTargetType::Color;
            renderTarget.loadOp = LoadOp::Clear;
            renderTarget.storeOp = StoreOp::Store;
            renderTarget.textureView = m_RenderTexture->getTextureView();

            RenderPassDesc renderPassDesc;
            renderPassDesc.renderArea = camera.getBounds();
            renderPassDesc.colorTargets.add(&renderTarget);

            cmdBuffer->beginDebugGroup(m_DebugName, m_DebugColor);

            TextureBarrier toTargetOutputBarrier;
            toTargetOutputBarrier.texture = m_RenderTexture;
            toTargetOutputBarrier.sourceAccess = ResourceAccess::ShaderRead;
            toTargetOutputBarrier.destAccess = ResourceAccess::ColorTargetWrite;
            toTargetOutputBarrier.destState = ResourceState::ColorTarget;

            TextureBarrier toShaderReadBarrier;
            toShaderReadBarrier.texture = m_RenderTexture;
            toShaderReadBarrier.sourceAccess = ResourceAccess::ColorTargetWrite;
            toShaderReadBarrier.destAccess = ResourceAccess::ShaderRead;
            toShaderReadBarrier.destState = ResourceState::ShaderRead;

            cmdBuffer->textureBarriers(toTargetOutputBarrier);

            cmdBuffer->beginRenderPass(renderPassDesc);
            cmdBuffer->pushConstant(m_Shader, ShaderStage::Vertex, projection);
            cmdBuffer->bindVertexBuffer(m_VertexBuffer, 0);
            cmdBuffer->bindIndexBuffer(m_IndexBuffer, 0, IndexFormat::UInt32);
            cmdBuffer->bindRenderPipeline(m_Pipeline);
            cmdBuffer->bindBindingGroup(m_BindingGroup);
            cmdBuffer->setViewport(Viewport::fromCamera(camera));
            cmdBuffer->setScissor(Scissor::fromCamera(camera));
            cmdBuffer->drawIndexed(m_QuadIndices.count(), 1, 0, 0, 0);
            cmdBuffer->endRenderPass();

            cmdBuffer->textureBarriers(toShaderReadBarrier);

            cmdBuffer->endDebugGroup();
            cmdBuffer->end();

            static uint64_t fenceValue = 0;

            FenceSignal signal;
            signal.fence = m_Fence;
            signal.stages = PipelineStages::ColorTargetOutput;
            signal.value = ++fenceValue;

            QueueExecuteInfo execInfo;
            execInfo.cmdBuffers = {cmdBuffer};
            execInfo.signals = {signal};
            renderQueue->executeCommandBuffers(execInfo);

            m_Fence->waitOnCPU(fenceValue);
        }


        m_ReadyToRender = false;
        return m_RenderTexture;
    }

    void FRenderer2D::resize(uint32_t width, uint32_t height)
    {
        m_Device->waitIdle();
        m_RenderTexture->resize(width, height);
    }

    void FRenderer2D::addQuad(const FVector2f& position, const FVector2f& size, const float rotation, const Color& color, const QuadMode quadMode, const uint32_t textureId)
    {
        FMatrix3f transform;
        transform = rotate(transform, FAxisAnglef(FVector3f::Forward, rotation));
        transform = translate(transform, position);

        const uint32_t lastVertexCount = m_QuadVertices.count();

        const QuadVertex quadVertices[]
        {
            /* BL */{ transform * FVector2f(0.0f, 0.0f), FVector2f(0.0f, 1.0f), color, quadMode, textureId },
            /* TL */{ transform * FVector2f(0.0f, size.y), FVector2f(1.0f, 1.0f), color, quadMode, textureId },
            /* TR */{ transform * FVector2f(size.x, size.y), FVector2f(1.0f, 0.0f), color, quadMode, textureId },
            /* BR */{ transform * FVector2f(size.x, 0.0f), FVector2f(0.0f, 0.0f), color, quadMode, textureId },
        };
        m_QuadVertices.addRange(quadVertices);

        const uint32_t quadIndices[]
        {
            0 + lastVertexCount, 2 + lastVertexCount, 1 + lastVertexCount,
            0 + lastVertexCount, 3 + lastVertexCount, 2 + lastVertexCount
        };
        m_QuadIndices.addRange(quadIndices);
    }

    uint32_t FRenderer2D::getOrAddTexture(const Texture* texture)
    {
        LUMA_ASSERT(texture, "ITexture should be valid!");
        if (m_Textures.contains(texture))
            return m_Textures.find(texture);
        m_Textures.add(texture);
        return m_Textures.count() - 1;
    }

    void FRenderer2D::drawQuad(const FVector2f& position, const FVector2f& size, const float rotation, const Color& color)
    {
        addQuad(position, size, rotation, color, QuadMode::Quad, 0);
    }

    void FRenderer2D::drawQuad(const FRect2f& rect, const float rotation, const Color& color)
    {
        const FVector2f position = { rect.x, rect.y };
        const FVector2f size = { rect.width, rect.height };
        drawQuad(position, size, rotation, color);
    }

    void FRenderer2D::drawEllipse(const FVector2f& position, const FVector2f& size, const float rotation, const Color& color)
    {
        addQuad(position, size, rotation, color, QuadMode::Ellipse, 0);
    }

    void FRenderer2D::drawEllipse(const FRect2f& rect, const float rotation, const Color& color)
    {
        const FVector2f position = { rect.x, rect.y };
        const FVector2f size = { rect.width, rect.height };
        drawEllipse(position, size, rotation, color);
    }

    void FRenderer2D::drawEllipseCentered(const FVector2f& position, const FVector2f& size, float rotation,
        const Color& color)
    {
        const FVector2f newPos = { position.x - size.x * 0.5f, position.y - size.y * 0.5f };
        drawEllipse(newPos, size, rotation, color);
    }

    void FRenderer2D::drawCircleCentered(const FVector2f& position, float radius, const Color& color)
    {
        const FVector2f newPos = { position.x - radius, position.y - radius };
        drawCircle(newPos, radius, color);
    }

    void FRenderer2D::drawCircle(const FVector2f& position, float radius, const Color& color)
    {
        drawEllipse(position, {radius * 2.0f, radius * 2.0f}, 0.0f, color);
    }

    void FRenderer2D::drawText(const FStringView text, const FVector2f& position, const float fontSize, const Color& color)
    {
        const TextParams params
        {
            .alignment = TextAlignment::Left,
            .style = TextStyle::Regular,
            .characterSpacing = 1.0f,
            .lineSpacing = 1.0f,
            .fontSize = fontSize
        };

        drawText(text, position, 0.0f, color, params);
    }

    void FRenderer2D::drawTextCentered(FStringView text, const FVector2<float>& position, float fontSize, const Color& color)
    {
        const float width = m_Font->getTextWidth(text, fontSize);
        const float height = m_Font->getTextHeight(text, fontSize);
        const float x = position.x - width * 0.5f;
        const float y = position.y - height;
        drawText(text, {x, y}, fontSize, color);
    }

    void FRenderer2D::drawText(const FStringView text, const FVector2f& position, const float rotation, const Color& color, TextParams params)
    {
        if (!m_Font) return;
        WeakRef<Texture> atlasTexture = m_Font->getAtlasTexture();
        if (!atlasTexture) return;

        const uint32_t textureId = getOrAddTexture(atlasTexture);

        const FontMetrics metrics = m_Font->getMetrics();
        const double fsScale = params.fontSize / (metrics.ascenderY - metrics.descenderY);

        double posX = 0.0;
        double posY = fsScale * metrics.ascenderY;

        FMatrix3f transform = FMatrix3f::Identity;
        transform = rotate(transform, {FVector3f::Forward, rotation});
        transform = translate(transform, position);

        for (size_t index = 0; index < text.count(); index++)
        {
            const auto character = m_Font->hasGlyph(text[index]) ? text[index] : '?';

            if (!m_Font->hasGlyph(character))
                continue;

            if (character == L'\r')
                continue;

            if (character == L'\t')
            {
                posX += 4.0 * m_Font->getAdvance(' ') * params.characterSpacing;
                continue;
            }

            if (character == L'\n')
            {
                posY += fsScale * metrics.lineHeight * params.lineSpacing;
                continue;
            }

            double tcl, tcb, tcr, tct;
            m_Font->getAtlasTextureCoordinates(character, tcl, tcr, tct, tcb);

            double pl, pb, pr, pt;
            m_Font->getPlaneBounds(character, pl, pr, pt, pb);

            const uint32_t lastVertexCount = m_QuadVertices.count();

            const QuadVertex quadVertices[]
            {
                {transform * FVector2f(posX + fsScale * pl, posY - fsScale * pb), FVector2f(tcl, tcb), color, QuadMode::Text, textureId},
                {transform * FVector2f(posX + fsScale * pl, posY - fsScale * pt), FVector2f(tcl, tct), color, QuadMode::Text, textureId},
                {transform * FVector2f(posX + fsScale * pr, posY - fsScale * pt), FVector2f(tcr, tct), color, QuadMode::Text, textureId},
                {transform * FVector2f(posX + fsScale * pr, posY - fsScale * pb), FVector2f(tcr, tcb), color, QuadMode::Text, textureId},
            };
            m_QuadVertices.addRange(quadVertices);

            const uint32_t quadIndices[]
            {
                0 + lastVertexCount, 2 + lastVertexCount, 1 + lastVertexCount,
                0 + lastVertexCount, 3 + lastVertexCount, 2 + lastVertexCount
            };
            m_QuadIndices.addRange(quadIndices);

            if (index != text.count() - 1)
            {
                const auto nextCharacter = text[index + 1];
                const double advance = m_Font->getAdvance(character, nextCharacter);
                posX += fsScale * advance * params.characterSpacing;
            }
        }
    }

    void FRenderer2D::drawSprite(const Sprite& sprite, const FVector2f& position, const float rotation, const Color& color)
    {
        if (!sprite.texture) return;
        const uint32_t textureId = getOrAddTexture(sprite.texture);

        FMatrix3f transform;
        transform = rotate(transform, FAxisAnglef(FVector3f::Forward, rotation));
        transform = translate(transform, position);

        const FVector2f topLeft     = FVector2f(0.0f, sprite.height);
        const FVector2f topRight    = FVector2f(sprite.width, sprite.height);
        const FVector2f bottomLeft  = FVector2f(0.0f, 0.0f);
        const FVector2f bottomRight = FVector2f(sprite.width, 0.0f);

        const uint32_t lastVertexCount = m_QuadVertices.count();

        const QuadVertex quadVertices[]
        {
            { transform * bottomLeft,  FVector2f(sprite.x / sprite.width, (sprite.y + sprite.height) / sprite.height), color, QuadMode::Sprite, textureId },
            { transform * topLeft,     FVector2f((sprite.x + sprite.width) / sprite.width, (sprite.y + sprite.height) / sprite.height), color, QuadMode::Sprite, textureId },
            { transform * topRight,    FVector2f((sprite.x + sprite.width) / sprite.width, sprite.y / sprite.height), color, QuadMode::Sprite, textureId },
            { transform * bottomRight, FVector2f(sprite.x / sprite.width, sprite.y / sprite.height), color, QuadMode::Sprite, textureId },
        };
        m_QuadVertices.addRange(quadVertices);

        const uint32_t quadIndices[]
        {
            0 + lastVertexCount, 2 + lastVertexCount, 1 + lastVertexCount,
            0 + lastVertexCount, 3 + lastVertexCount, 2 + lastVertexCount
        };
        m_QuadIndices.addRange(quadIndices);
    }

    void FRenderer2D::setFont(Ref<Font> font)
    {
        m_Font = font ? font : m_DefaultFont;
    }

    void FRenderer2D::setDebugName(const FString& debugName)
    {
        m_DebugName = debugName;
    }

    void FRenderer2D::setDebugColor(const Color& debugColor)
    {
        m_DebugColor = debugColor;
    }

    Ref<Texture> FRenderer2D::getRenderTexture() const
    {
        return m_RenderTexture;
    }
}
