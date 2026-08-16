#include "Luma/Rendering/Renderer2D.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/GraphicsPipeline.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Rendering/BindingSet.h"
#include "Luma/Rendering/RenderDevice.h"
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

namespace Luma
{
    FRenderer2D::FRenderer2D(Ref<IRenderDevice> renderDevice)
    {
        initialize(renderDevice);
    }

    bool FRenderer2D::initialize(Ref<IRenderDevice> renderDevice)
    {
        if (!renderDevice) return false;
        m_RenderDevice = renderDevice;

        m_DefaultFont = Ref<FFont>::create();
        m_DefaultFont->loadAndGenerate(robotoFontData, EFontAtlasType::MSDF, {FCharacterSet::ascii()}, renderDevice);
        setFont(m_DefaultFont);

        FShaderDesc shaderDesc;
        shaderDesc.moduleName = "Renderer2D";
        shaderDesc.stageFlags = EShaderStageBits::Vertex | EShaderStageBits::Fragment;
        shaderDesc.filepath = FPath::getEngineShaderPath("Renderer2D.slang");
        m_Shader = m_RenderDevice->createShader(shaderDesc);
        if (!m_Shader) return false;

        FVertexInputLayout vertexLayout;
        vertexLayout.addInputBinding(0, EVertexInputRate::Vertex);
        vertexLayout.addInputAttribute({"POSITION", EShaderDataType::Float2, 0});
        vertexLayout.addInputAttribute({"TEXCOORD", EShaderDataType::Float2, 0});
        vertexLayout.addInputAttribute({"COLOR", EShaderDataType::Float4, 0});
        vertexLayout.addInputAttribute({"MODE", EShaderDataType::UInt, 0});
        vertexLayout.addInputAttribute({"TEXID", EShaderDataType::UInt, 0});

        FGraphicsPipelineDesc gpDesc;
        gpDesc.device = m_RenderDevice;
        gpDesc.shaderProgram = m_Shader;
        gpDesc.rasterization.cullMode = ECullMode::None;
        gpDesc.colorFormatCount = 1;
        gpDesc.colorFormats[0] = EFormat::R8G8B8A8_SRGB;
        gpDesc.colorBlend[0] = FColorBlendState(true, FBlendFunction::alphaBlend());
        gpDesc.inputLayout = vertexLayout;
        gpDesc.depthStencil.depthTestEnable = false;
        gpDesc.depthStencil.depthWriteEnable = false;
        gpDesc.depthStencil.stencilTestEnable = false;
        m_Pipeline = m_RenderDevice->createGraphicsPipeline(gpDesc);
        if (!m_Pipeline) return false;

        FBufferDesc vbDesc;
        vbDesc.alwaysMapped = true;
        vbDesc.usage = EBufferUsage::VertexBuffer;
        vbDesc.size = MAX_QUAD * 4 * sizeof(QuadVertex);
        m_VertexBuffer = m_RenderDevice->createBuffer(vbDesc);
        if (!m_VertexBuffer) return false;

        FBufferDesc ibDesc;
        ibDesc.alwaysMapped = true;
        ibDesc.usage = EBufferUsage::IndexBuffer;
        ibDesc.size = MAX_QUAD * 6 * sizeof(uint32_t);
        m_IndexBuffer = m_RenderDevice->createBuffer(ibDesc);
        if (!m_IndexBuffer) return false;

        FSamplerDesc samplerDesc = FSamplerDesc();
        samplerDesc.magFilter = EFilter::Linear;
        samplerDesc.magFilter = EFilter::Linear;
        m_Sampler = m_RenderDevice->getOrCreateSampler(samplerDesc);
        if (!m_Sampler) return false;

        samplerDesc.magFilter = EFilter::Nearest;
        samplerDesc.minFilter = EFilter::Nearest;
        m_SpriteSampler = m_RenderDevice->getOrCreateSampler(samplerDesc);

        m_BindingSet = m_Shader->createBindingSet(0);
        if (!m_BindingSet) return false;

        m_BindingSet->bindSampler(0, m_SpriteSampler);
        m_BindingSet->bindSampler(1, m_Sampler);
        m_LocalToWorldMatrix = FMatrix4f::Identity;
        return true;
    }

    void FRenderer2D::destroy()
    {
        m_RenderDevice->waitIdle();
        m_RenderDevice = nullptr;
        m_DefaultFont = nullptr;
        m_Font = nullptr;
        m_Shader = nullptr;
        m_Pipeline = nullptr;
        m_VertexBuffer = nullptr;
        m_IndexBuffer = nullptr;
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

        m_BindingSet->bindTextures(2, m_Textures, EBindingType::SampledTexture);
        m_BeginDrawing = false;
        m_ReadyToRender = true;
    }

    void FRenderer2D::render(ICommandBuffer* cmdBuffer, uint32_t width, uint32_t height)
    {
        LUMA_ASSERT(m_ReadyToRender, "not ready to render yet!!");

        const FMatrix4f projection = scale(orthoTopLeft(static_cast<float>(width), static_cast<float>(height), 1.0f, -1.0f, 1.0f), {1.0f, -1.0f, 1.0f});
        const FMatrix4f mvp = projection * m_LocalToWorldMatrix;
        cmdBuffer->beginDebugGroup(m_DebugName, m_DebugColor);
        cmdBuffer->pushConstants(m_Shader, EShaderStageBits::Vertex, &mvp, 0, sizeof(FMatrix4f));
        cmdBuffer->bindVertexBuffer(m_VertexBuffer, 0);
        cmdBuffer->bindIndexBuffer(m_IndexBuffer, 0, EIndexFormat::Uint32);
        cmdBuffer->bindGraphicsPipeline(m_Pipeline);
        cmdBuffer->bindBindingSet(m_BindingSet, m_Shader);
        cmdBuffer->setViewport(FViewport(0.0f, 0.0f, width, height, 0.0f, 1.0f));
        cmdBuffer->setScissor(FScissor(0, 0, width, height));
        cmdBuffer->drawIndexed(m_QuadIndices.count(), 1, 0, 0, 0);
        cmdBuffer->endDebugGroup();
        m_ReadyToRender = false;
    }

    void FRenderer2D::addQuad(const FVector2f& position, const FVector2f& size, const float rotation, const FColor& color, const QuadMode quadMode, const uint32_t textureId)
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

    uint32_t FRenderer2D::getOrAddTexture(const ITexture* texture)
    {
        LUMA_ASSERT(texture, "ITexture should be valid!");
        if (m_Textures.contains(texture))
            return m_Textures.find(texture);
        m_Textures.add(texture);
        return m_Textures.count() - 1;
    }

    void FRenderer2D::drawQuad(const FVector2f& position, const FVector2f& size, const float rotation, const FColor& color)
    {
        addQuad(position, size, rotation, color, QuadMode::Quad, 0);
    }

    void FRenderer2D::drawQuad(const FRect2f& rect, const float rotation, const FColor& color)
    {
        const FVector2f position = { rect.x, rect.y };
        const FVector2f size = { rect.width, rect.height };
        drawQuad(position, size, rotation, color);
    }

    void FRenderer2D::drawEllipse(const FVector2f& position, const FVector2f& size, const float rotation, const FColor& color)
    {
        addQuad(position, size, rotation, color, QuadMode::Ellipse, 0);
    }

    void FRenderer2D::drawEllipse(const FRect2f& rect, const float rotation, const FColor& color)
    {
        const FVector2f position = { rect.x, rect.y };
        const FVector2f size = { rect.width, rect.height };
        drawEllipse(position, size, rotation, color);
    }

    void FRenderer2D::drawEllipseCentered(const FVector2f& position, const FVector2f& size, float rotation,
        const FColor& color)
    {
        const FVector2f newPos = { position.x - size.x * 0.5f, position.y - size.y * 0.5f };
        drawEllipse(newPos, size, rotation, color);
    }

    void FRenderer2D::drawCircleCentered(const FVector2f& position, float radius, const FColor& color)
    {
        const FVector2f newPos = { position.x - radius, position.y - radius };
        drawCircle(newPos, radius, color);
    }

    void FRenderer2D::drawCircle(const FVector2f& position, float radius, const FColor& color)
    {
        drawEllipse(position, {radius * 2.0f, radius * 2.0f}, 0.0f, color);
    }

    void FRenderer2D::drawText(const FStringView text, const FVector2f& position, const float fontSize, const FColor& color)
    {
        const TextParams params
        {
            .alignment = ETextAlignment::Left,
            .style = ETextStyleBits::Regular,
            .characterSpacing = 1.0f,
            .lineSpacing = 1.0f,
            .fontSize = fontSize
        };

        drawText(text, position, 0.0f, color, params);
    }

    void FRenderer2D::drawTextCentered(FStringView text, const FVector2<float>& position, float fontSize, const FColor& color)
    {
        const float width = m_Font->getTextWidth(text, fontSize);
        const float height = m_Font->getTextHeight(text, fontSize);
        const float x = position.x - width * 0.5f;
        const float y = position.y - height;
        drawText(text, {x, y}, fontSize, color);
    }

    void FRenderer2D::drawText(const FStringView text, const FVector2f& position, const float rotation, const FColor& color, TextParams params)
    {
        if (!m_Font) return;
        WeakRef<ITexture> atlasTexture = m_Font->getAtlasTexture();
        if (!atlasTexture) return;

        const uint32_t textureId = getOrAddTexture(atlasTexture);

        const FFontMetrics metrics = m_Font->getMetrics();
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

    void FRenderer2D::drawSprite(const Sprite& sprite, const FVector2f& position, const float rotation, const FColor& color)
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

    void FRenderer2D::setFont(Ref<FFont> font)
    {
        if (!font)
        {
            m_Font = m_DefaultFont;
            return;
        }
        m_Font = font;
    }

    void FRenderer2D::setLocalToWorldMatrix(const FMatrix4f& localToWorld)
    {
        m_LocalToWorldMatrix = localToWorld;
    }

    void FRenderer2D::setDebugName(const FString& debugName)
    {
        m_DebugName = debugName;
    }

    void FRenderer2D::setDebugColor(const FColor& debugColor)
    {
        m_DebugColor = debugColor;
    }
}
