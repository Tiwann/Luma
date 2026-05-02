#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Math/Color.h"
#include "Luma/Math/Matrix4.h"
#include "Luma/Math/Rect2.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Runtime/Sprite.h"
#include "Luma/Runtime/TextAlignment.h"
#include "Luma/Runtime/TextStyle.h"
#include "Luma/Rendering/RenderDevice.h"
#include "Luma/Rendering/Sampler.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/BindingSet.h"

namespace Luma
{
    struct TextParams
    {
        ETextAlignment alignment = ETextAlignment::Left;
        FTextStyleFlags style = ETextStyleBits::None;
        float characterSpacing = 0.0f;
        float lineSpacing = 0.0f;
        float fontSize = 10.0f;
    };

    class FRenderer2D : public IRefCounted<FRenderer2D>
    {
    public:
        FRenderer2D() = default;
        ~FRenderer2D() override = default;

        bool initialize(Ref<IRenderDevice> renderDevice);
        void destroy();

        /// Begins a new batch
        void begin();

        /// Ends the current batch and upload data to GPU
        void end();

        // Render and flush the batches
        void render(ICommandBuffer* cmdBuffer, uint32_t width, uint32_t height);

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

        /// Draw a colored ellipse
        /// @param position Position of the top left corner of the circumscribed square of the circle
        /// @param radius Radius of the circle
        /// @param color Color to fill the circle with
        void drawCircle(const FVector2f& position, float radius, const FColor& color);


        void drawText(FStringView text, const FVector2f& position, float fontSize, const FColor& color);
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
        //void SetFont(Ref<Font> font);

        /// Sets the world space matrix
        /// @param localToWorld World space matrix to use
        void setLocalToWorldMatrix(const FMatrix4f& localToWorld);
    private:
        Ref<IRenderDevice> m_RenderDevice = nullptr;
        Ref<IShader> m_Shader = nullptr;
        Ref<IGraphicsPipeline> m_Pipeline = nullptr;
        Ref<ISampler> m_Sampler = nullptr;
        Ref<ISampler> m_SpriteSampler = nullptr;
        Ref<IBuffer> m_VertexBuffer = nullptr;
        Ref<IBuffer> m_IndexBuffer = nullptr;
        Ref<IBindingSet> m_BindingSet = nullptr;
        //Ref<Font> m_Font = nullptr;
        FMatrix4f m_LocalToWorldMatrix;
        static const FMatrix4f DefaultLocalToWorldMatrix;
    };

}
