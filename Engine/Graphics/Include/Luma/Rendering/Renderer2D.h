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
#include "Luma/Rendering/Device.h"
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
        TextAlignment alignment = TextAlignment::Left;
        FTextStyleFlags style = TextStyle::Regular;
        float characterSpacing = 0.0f;
        float lineSpacing = 0.0f;
        float fontSize = 10.0f;
    };

    class Renderer2D : public RefCounted<Renderer2D>
    {
        static constexpr uint32_t MAX_QUAD = Math::sqr(512);
    public:
        Renderer2D() = default;
        explicit Renderer2D(Ref<RHI::Device> device, uint32_t width, uint32_t height);
        ~Renderer2D() override = default;

        void destroy();

        /// Begins a new batch
        void begin();

        /// Ends the current batch and upload data to GPU
        void end();

        // Render and flush the batches
        Ref<RHI::Texture> render(const Camera& camera);

        // Resize the internal render texture and camera
        void resize(uint32_t width, uint32_t height);

        /// Draw a colored quad
        /// @param position Position in screen space
        /// @param size size of the quad in screen space
        /// @param rotation Rotation of the quad in radians
        /// @param color Color to fill the quad with
        void drawQuad(const FVector2f& position, const FVector2f& size, float rotation, const Color& color);

        /// Draw a colored quad
        /// @param rect Rectangle to draw (position + size)
        /// @param rotation Rotation of the quad to draw in radians
        /// @param color Color to fill the quad with
        void drawQuad(const FRect2f& rect, float rotation, const Color& color);

        /// Draw a colored ellipse
        /// @param position Position of the top left corner of the circumscribed rectangle of the ellipse
        /// @param size Size of the ellipse
        /// @param rotation Rotation in radians of the ellipse
        /// @param color Color to fill the ellipse with
        void drawEllipse(const FVector2f& position, const FVector2f& size, float rotation, const Color& color);

        /// Draw a colored ellipse
        /// @param rect Circumscribed rectangle of the ellipse
        /// @param rotation Rotation in radians of the ellipse
        /// @param color Color to fill the ellipse with
        void drawEllipse(const FRect2f& rect, float rotation, const Color& color);

        // Draw a colored ellipse
        /// @param position Position of the center of the ellipse
        /// @param size Size of the ellipse
        /// @param rotation Rotation in radians of the ellipse
        /// @param color Color to fill the ellipse with
        void drawEllipseCentered(const FVector2f& position, const FVector2f& size, float rotation, const Color& color);

        /// Draw a colored ellipse
        /// @param position Position of the top left corner of the circumscribed square of the circle
        /// @param radius Radius of the circle
        /// @param color Color to fill the circle with
        void drawCircle(const FVector2f& position, float radius, const Color& color);

        /// Draw a colored ellipse
        /// @param position Position of the center of the circle
        /// @param radius Radius of the circle
        /// @param color Color to fill the circle with
        void drawCircleCentered(const FVector2f& position, float radius, const Color& color);


        void drawText(StringView text, const FVector2f& position, float fontSize, const Color& color);
        void drawTextCentered(StringView text, const FVector2<float>& position, float fontSize, const Color& color);

        /// Draw a colored text
        /// @param text Text to draw
        /// @param position Position of the text in screen space
        /// @param rotation Rotation of the text
        /// @param color Color of the text
        /// @param params
        void drawText(StringView text, const FVector2f& position, float rotation, const Color& color, TextParams params);

        /// Draw a sprite
        /// @param sprite Sprite to draw
        /// @param position Position in screen space
        /// @param rotation Rotation of the sprite
        /// @param color Color of the sprite
        void drawSprite(const Sprite& sprite, const FVector2f& position, float rotation, const Color& color);

        /// Sets the current font
        /// @param font Font asset to use. Null will assign the default font.
        void setFont(Ref<Font> font);

        void setDebugName(const String& debugName);
        void setDebugColor(const Color& debugColor);

        Ref<RHI::Texture> getRenderTexture() const;
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

        void addQuad(const FVector2f& position, const FVector2f& size, float rotation, const Color& color, QuadMode quadMode, uint32_t textureId);
        uint32_t getOrAddTexture(const RHI::Texture* texture);

        Ref<RHI::Device> m_Device = nullptr;
        Ref<RHI::Shader> m_Shader = nullptr;
        Ref<RHI::RenderPipeline> m_Pipeline = nullptr;
        Ref<RHI::Sampler> m_Sampler = nullptr;
        Ref<RHI::Sampler> m_SpriteSampler = nullptr;
        Ref<RHI::Buffer> m_VertexBuffer = nullptr;
        Ref<RHI::Buffer> m_IndexBuffer = nullptr;
        Ref<RHI::BindingGroup> m_BindingGroup = nullptr;
        Ref<RHI::Fence> m_Fence = nullptr;
        Ref<Font> m_Font = nullptr;
        Ref<RHI::Texture> m_RenderTexture = nullptr;
        String m_DebugName = "Renderer2D";
        Color m_DebugColor = Color::Cyan;

        bool m_BeginDrawing = false;
        bool m_ReadyToRender = false;
        Ref<Font> m_DefaultFont = nullptr;
        Array<QuadVertex> m_QuadVertices;
        Array<uint32_t> m_QuadIndices;
        Array<const RHI::Texture*> m_Textures;
    };

}
