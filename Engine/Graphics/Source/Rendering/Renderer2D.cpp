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

namespace Luma
{
    const FMatrix4f FRenderer2D::DefaultLocalToWorldMatrix = FMatrix4f::Identity;

    enum class QuadMode
    {
        Quad,
        Ellipse,
        Text,
        Sprite
    };

    struct QuadVertex
    {
        FVector2f position;
        FVector2f uv;
        FVector4f color;
        QuadMode quadMode;
        uint32_t textureId;
    };

    struct Batch
    {
        TArray<QuadVertex> vertices;
        TArray<uint32_t> indices;
        TArray<WeakRef<ITexture>> textures;
    };

    static constexpr uint32_t MAX_BATCHES = 10;
    static constexpr uint32_t MAX_QUAD = 1024;
    static bool BeginDrawing = false;
    static bool ReadyToRender = false;
    static TArray<QuadVertex> QuadVertices;
    static TArray<uint32_t> QuadIndices;
    static TArray<const ITexture*> Textures;

    bool FRenderer2D::initialize(Ref<IRenderDevice> renderDevice)
    {
        if (!renderDevice) return false;
        m_RenderDevice = renderDevice;

        FShaderDesc shaderDesc;
        shaderDesc.moduleName = "Renderer2D";
        shaderDesc.stageFlags = TFlags(EShaderStageBits::Vertex) | EShaderStageBits::Fragment;
        shaderDesc.filepath = FPath::getEngineAssetPath("Shaders/Renderer2D.slang");
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
        gpDesc.colorFormatCount = 1;
        gpDesc.colorFormats[0] = EFormat::R8G8B8A8_SRGB;
        gpDesc.colorBlend[0] = {true};
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

        FSamplerDesc createInfo = FSamplerDesc();
        m_Sampler = m_RenderDevice->getOrCreateSampler(createInfo);
        if (!m_Sampler) return false;

        createInfo.magFilter = EFilter::Nearest;
        createInfo.minFilter = EFilter::Nearest;
        m_SpriteSampler = m_RenderDevice->getOrCreateSampler(createInfo);

        m_BindingSet = m_Shader->createBindingSet(0);
        if (!m_BindingSet) return false;

        m_BindingSet->bindSampler(0, m_Sampler);
        m_LocalToWorldMatrix = DefaultLocalToWorldMatrix;
        return true;
    }

    void FRenderer2D::destroy()
    {
        m_RenderDevice = nullptr;
        //m_Font = nullptr;
        if (m_Shader) m_Shader->destroy();
        if (m_Pipeline) m_Pipeline->destroy();
        if (m_VertexBuffer) m_VertexBuffer->destroy();
        if (m_IndexBuffer) m_IndexBuffer->destroy();
        QuadVertices.free();
        QuadIndices.free();
    }


    void FRenderer2D::begin()
    {
        LUMA_ASSERT(!BeginDrawing, "begin/end mismatch");
        BeginDrawing = true;
        ReadyToRender = false;
        QuadVertices.clear();
        QuadIndices.clear();
        Textures.clear();
    }

    void FRenderer2D::end()
    {
        LUMA_ASSERT(BeginDrawing, "begin/end mismatch");

        void* vertexMapped = m_VertexBuffer->map();
        memcpy(vertexMapped, QuadVertices.data(), QuadVertices.size());
        m_VertexBuffer->unmap(vertexMapped);

        void* indexMapped = m_IndexBuffer->map();
        memcpy(indexMapped, QuadIndices.data(), QuadIndices.size());
        m_IndexBuffer->unmap(indexMapped);

        m_BindingSet->bindTextures(1, Textures, EBindingType::SampledTexture);
        BeginDrawing = false;
        ReadyToRender = true;
    }

    void FRenderer2D::render(ICommandBuffer* cmdBuffer, uint32_t width, uint32_t height)
    {
        LUMA_ASSERT(ReadyToRender, "not ready to render yet!!");

        const FMatrix4f projection = scale(orthoTopLeft<float>(width, height, 1.0f, -1.0, 1.0f), {1.0f, -1.0f, 1.0f});
        const FMatrix4f mvp = projection * m_LocalToWorldMatrix;
        cmdBuffer->beginDebugGroup("FRenderer2D", FColor::Orange);
        cmdBuffer->pushConstants(m_Shader, EShaderStageBits::Vertex, mvp.valuePtr(), 0, sizeof(FMatrix4f));
        cmdBuffer->bindVertexBuffer(m_VertexBuffer, 0);
        cmdBuffer->bindIndexBuffer(m_IndexBuffer, 0, EIndexFormat::Uint32);
        cmdBuffer->bindGraphicsPipeline(m_Pipeline);
        cmdBuffer->bindBindingSet(m_BindingSet, m_Shader);
        cmdBuffer->setViewport(FViewport(0.0f, 0.0f, width, height, 0.0f, 1.0f));
        cmdBuffer->setScissor(FScissor(0, 0, width, height));
        cmdBuffer->drawIndexed(QuadIndices.count(), 1, 0, 0, 0);
        cmdBuffer->endDebugGroup();
        ReadyToRender = false;
    }

    static void addQuad(const FVector2f& position, const FVector2f& size, const float rotation, const FColor& color, const QuadMode quadMode, const uint32_t textureId)
    {
        FMatrix3f transform;
        transform = rotate(transform, FAxisAnglef(FVector3f::Forward, rotation));
        transform = translate(transform, position);

        const uint32_t lastVertexCount = QuadVertices.count();

        const QuadVertex quadVertices[]
        {
            /* BL */{ transform * FVector2f(0.0f, 0.0f), FVector2f(0.0f, 1.0f), color, quadMode, textureId },
            /* TL */{ transform * FVector2f(0.0f, size.y), FVector2f(1.0f, 1.0f), color, quadMode, textureId },
            /* TR */{ transform * FVector2f(size.x, size.y), FVector2f(1.0f, 0.0f), color, quadMode, textureId },
            /* BR */{ transform * FVector2f(size.x, 0.0f), FVector2f(0.0f, 0.0f), color, quadMode, textureId },
        };
        QuadVertices.addRange(quadVertices);

        const uint32_t quadIndices[]
        {
            0 + lastVertexCount, 2 + lastVertexCount, 1 + lastVertexCount,
            0 + lastVertexCount, 3 + lastVertexCount, 2 + lastVertexCount
        };
        QuadIndices.addRange(quadIndices);
    }

    static uint32_t getOrAddTexture(const ITexture* texture)
    {
        LUMA_ASSERT(texture, "ITexture should be valid!");
        if (Textures.contains(texture))
            return Textures.find(texture);
        Textures.add(texture);
        return Textures.count() - 1;
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

    void FRenderer2D::drawCircle(const FVector2f& position, float radius, const FColor& color)
    {
        drawEllipse(position, {radius, radius}, 0.0f, color);
    }

    /*void FRenderer2D::drawText(const StringView text, const FVector2f& position, const float fontSize, const FColor& color)
    {
        const TextParams params
        {
            .alignment = TextAlignment::Left,
            .style = TextStyleBits::None,
            .characterSpacing = 0.0f,
            .lineSpacing = 0.0f,
            .fontSize = fontSize
        };

        drawText(text, position, 0.0f, color, params);
    }*/

    /*void FRenderer2D::drawText(const StringView text, const FVector2f& position, const float rotation, const FColor& color, TextParams params)
    {
        using namespace msdf_atlas;

        if (!m_Font) return;
        if (!m_Font->GetAtlasTexture()) return;
        WeakRef<ITexture> atlasTexture = m_Font->GetAtlasTexture();

        const uint32_t textureId = GetOrAddTexture(atlasTexture);
        const FontGeometry& fontGeometry = m_Font->GetFontGeometry();
        const auto& metrics = fontGeometry.getMetrics();

        float posX = 0.0f, posY = 0.0f;
        const WideString wideText = StringConvertToWide(text);
        const float textWidth = m_Font->CalculateTextWidth<wchar_t>(wideText, params.characterSpacing);
        const float fsScale = 1.0f / (metrics.ascenderY - metrics.descenderY);
        switch (params.alignment)
        {
        case TextAlignment::Left:
            posX = 0.0f;
            posY = fsScale * metrics.ascenderY;
            break;
        case TextAlignment::Center:
            posX = -0.5f * textWidth;
            posY = fsScale * metrics.ascenderY;
            break;
        case TextAlignment::Right:
            posX = textWidth;
            posY = fsScale * metrics.ascenderY;
            break;
        }

        FMatrix3f transform = FMatrix3f::Identity;
        transform.Scale(params.fontSize);
        transform.Rotate(Vector3::Forward, rotation);
        transform.Translate(position);

        for (size_t index = 0; index < wideText.Count(); index++)
        {
            const auto character = wideText[index];
            const GlyphGeometry* glyph = fontGeometry.getGlyph(character) ? fontGeometry.getGlyph(character)
            : fontGeometry.getGlyph('?') ? fontGeometry.getGlyph('?') : nullptr;
            if (!glyph) continue;

            if (character == L'\r')
                continue;

            if (character == L'\t')
            {
                posX += 4.0f * fontGeometry.getGlyph(' ')->getAdvance() * params.characterSpacing;
                continue;
            }

            if (character == L'\n')
            {
                switch (params.alignment)
                {
                case ETextAlignment::Left:
                    posX = 0.0f;
                    break;
                case ETextAlignment::Center:
                    posX = -0.5f * textWidth;
                    break;
                case ETextAlignment::Right:
                    posX = textWidth;
                    break;
                }

                posY += fsScale * metrics.lineHeight + params.lineSpacing;
                continue;
            }

            double tcl, tcb, tcr, tct;
            glyph->getQuadAtlasBounds(tcl, tcb, tcr, tct);
            tcl /= atlasTexture->getWidth();
            tcr /= atlasTexture->getWidth();
            tcb /= atlasTexture->getHeight();
            tct /= atlasTexture->getHeight();

            double pl, pb, pr, pt;
            glyph->getQuadPlaneBounds(pl, pb, pr, pt);

            const uint32_t lastVertexCount = QuadVertices.count();

            const QuadVertex quadVertices[]
            {
                {transform * FVector2f(posX + fsScale * pl, posY - fsScale * pb), FVector2f(tcl, tcb), color, QuadMode::Text, textureId},
                {transform * FVector2f(posX + fsScale * pl, posY - fsScale * pt), FVector2f(tcl, tct), color, QuadMode::Text, textureId},
                {transform * FVector2f(posX + fsScale * pr, posY - fsScale * pt), FVector2f(tcr, tct), color, QuadMode::Text, textureId},
                {transform * FVector2f(posX + fsScale * pr, posY - fsScale * pb), FVector2f(tcr, tcb), color, QuadMode::Text, textureId},
            };
            QuadVertices.addRange(quadVertices);

            const uint32_t quadIndices[]
            {
                0 + lastVertexCount, 2 + lastVertexCount, 1 + lastVertexCount,
                0 + lastVertexCount, 3 + lastVertexCount, 2 + lastVertexCount
            };
            QuadIndices.addRange(quadIndices);

            if (index != wideText.count() - 1)
            {
                const auto nextCharacter = wideText[index + 1];
                double advance = 0.0;
                fontGeometry.getAdvance(advance, character, nextCharacter);
                posX += fsScale * advance + params.characterSpacing;
            }
        }
    }*/

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

        const uint32_t lastVertexCount = QuadVertices.count();

        const QuadVertex quadVertices[]
        {
            { transform * bottomLeft,  FVector2f(sprite.x / sprite.width, (sprite.y + sprite.height) / sprite.height), color, QuadMode::Sprite, textureId },
            { transform * topLeft,     FVector2f((sprite.x + sprite.width) / sprite.width, (sprite.y + sprite.height) / sprite.height), color, QuadMode::Sprite, textureId },
            { transform * topRight,    FVector2f((sprite.x + sprite.width) / sprite.width, sprite.y / sprite.height), color, QuadMode::Sprite, textureId },
            { transform * bottomRight, FVector2f(sprite.x / sprite.width, sprite.y / sprite.height), color, QuadMode::Sprite, textureId },
        };
        QuadVertices.addRange(quadVertices);

        const uint32_t quadIndices[]
        {
            0 + lastVertexCount, 2 + lastVertexCount, 1 + lastVertexCount,
            0 + lastVertexCount, 3 + lastVertexCount, 2 + lastVertexCount
        };
        QuadIndices.addRange(quadIndices);
    }

    /*void FRenderer2D::SetFont(Ref<Font> font)
    {
        m_Font = font;
    }*/

    void FRenderer2D::setLocalToWorldMatrix(const FMatrix4f& localToWorld)
    {
        m_LocalToWorldMatrix = localToWorld;
    }
}
