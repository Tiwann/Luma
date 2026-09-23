#pragma once
#include "Luma/Asset/Font.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Math/Color.h"
#include "Luma/Math/Math.h"
#include "Luma/Math/Matrix4.h"
#include "Luma/Math/Rect2.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Runtime/Sprite.h"
#include "Luma/Runtime/TextAlignment.h"
#include "Luma/Runtime/TextStyle.h"
#include "Luma/Rendering/GPUDevice.h"
#include "Luma/Rendering/Sampler.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/RenderPipeline.h"
#include "Luma/Rendering/BindingGroup.h"
#include "Luma/Rendering/Texture.h"

namespace Luma
{
    struct TextParams
    {
        ETextAlignment alignment = ETextAlignment::Left;
        FTextStyleFlags style = ETextStyle::Regular;
        float characterSpacing = 0.0f;
        float lineSpacing = 0.0f;
        float fontSize = 10.0f;
    };

    class FRenderer2D : public IRefCounted<FRenderer2D>
    {
        static constexpr uint32_t MAX_QUAD = FMath::sqr(512);
    public:
        FRenderer2D() = default;
        explicit FRenderer2D(Ref<IGPUDevice> device, uint32_t width, uint32_t height);
        ~FRenderer2D() override = default;

        void destroy();

        /// Begins a new batch
        void begin();

        /// Ends the current batch and upload data to GPU
        void end();

        // Render and flush the batches
        Ref<ITexture> render(const FCamera& camera);

        // Resize the internal render texture and camera
        void resize(uint32_t width, uint32_t height);

        /// Draw a colored quad
        /// @param position Position in screen space
        /// @param size size of the quad in screen space
        /// @param rotation Rotation of the quad in radians
        /// @param color Color to fill the quad with
        void drawQuad(const FVector2f& position, const FVector2f& size, float rotation, const FColor& color);

        /// Draw a colored quad
        /// @param rect Rectangle to draw (position + size)
        /// @param rotation Rotation of the quad to draw in radians
        /// @param color Color to fill the quad with
        void drawQuad(const FRect2f& rect, float rotation, const FColor& color);

        /// Draw a colored ellipse
        /// @param position Position of the top left corner of the circumscribed rectangle of the ellipse
        /// @param size Size of the ellipse
        /// @param rotation Rotation in radians of the ellipse
        /// @param color Color to fill the ellipse with
        void drawEllipse(const FVector2f& position, const FVector2f& size, float rotation, const FColor& color);

        /// Draw a colored ellipse
        /// @param rect Circumscribed rectangle of the ellipse
        /// @param rotation Rotation in radians of the ellipse
        /// @param color Color to fill the ellipse with
        void drawEllipse(const FRect2f& rect, float rotation, const FColor& color);

        // Draw a colored ellipse
        /// @param position Position of the center of the ellipse
        /// @param size Size of the ellipse
        /// @param rotation Rotation in radians of the ellipse
        /// @param color Color to fill the ellipse with
        void drawEllipseCentered(const FVector2f& position, const FVector2f& size, float rotation, const FColor& color);

        /// Draw a colored ellipse
        /// @param position Position of the top left corner of the circumscribed square of the circle
        /// @param radius Radius of the circle
        /// @param color Color to fill the circle with
        void drawCircle(const FVector2f& position, float radius, const FColor& color);

        /// Draw a colored ellipse
        /// @param position Position of the center of the circle
        /// @param radius Radius of the circle
        /// @param color Color to fill the circle with
        void drawCircleCentered(const FVector2f& position, float radius, const FColor& color);


        void drawText(FStringView text, const FVector2f& position, float fontSize, const FColor& color);
        void drawTextCentered(FStringView text, const FVector2<float>& position, float fontSize, const FColor& color);

        /// Draw a colored text
        /// @param text Text to draw
        /// @param position Position of the text in screen space
        /// @param rotation Rotation of the text
        /// @param color Color of the text
        /// @param params
        void drawText(FStringView text, const FVector2f& position, float rotation, const FColor& color, TextParams params);

        /// Draw a sprite
        /// @param sprite Sprite to draw
        /// @param position Position in screen space
        /// @param rotation Rotation of the sprite
        /// @param color Color of the sprite
        void drawSprite(const Sprite& sprite, const FVector2f& position, float rotation, const FColor& color);

        /// Sets the current font
        /// @param font Font asset to use. Null will assign the default font.
        void setFont(Ref<FFont> font);

        void setDebugName(const FString& debugName);
        void setDebugColor(const FColor& debugColor);

        Ref<ITexture> getRenderTexture() const;
    private:
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

        void addQuad(const FVector2f& position, const FVector2f& size, float rotation, const FColor& color, QuadMode quadMode, uint32_t textureId);
        uint32_t getOrAddTexture(const ITexture* texture);

        Ref<IGPUDevice> m_GpuDevice = nullptr;
        Ref<IShader> m_ShaderProgram = nullptr;
        Ref<IRenderPipeline> m_Pipeline = nullptr;
        Ref<ISampler> m_Sampler = nullptr;
        Ref<ISampler> m_SpriteSampler = nullptr;
        Ref<IBuffer> m_VertexBuffer = nullptr;
        Ref<IBuffer> m_IndexBuffer = nullptr;
        Ref<IBindingGroup> m_BindingGroup = nullptr;
        Ref<IFence> m_Fence = nullptr;
        Ref<FFont> m_Font = nullptr;
        Ref<ITexture> m_RenderTexture = nullptr;
        FString m_DebugName = "Renderer2D";
        FColor m_DebugColor = FColor::Cyan;

        bool m_BeginDrawing = false;
        bool m_ReadyToRender = false;
        Ref<FFont> m_DefaultFont = nullptr;
        TArray<QuadVertex> m_QuadVertices;
        TArray<uint32_t> m_QuadIndices;
        TArray<const ITexture*> m_Textures;
    };

}
