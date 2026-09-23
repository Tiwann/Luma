#pragma once
#include "Luma/Memory/RefCounted.h"
#include "Luma/Math/Vector2.h"
#include <cstdint>

struct ImGuiContext;

namespace Luma
{
    struct Window;

    namespace RHI
    {
        struct Device;
        struct CommandBuffer;
        struct TextureView;
        struct Sampler;
    }

    struct ImguiRendererDesc
    {
        Window* window = nullptr;
        RHI::Device* device = nullptr;
        uint32_t sampleCount = 0;
    };

    struct ImguiRenderer : RefCounted<ImguiRenderer>
    {
        ImguiRenderer() = default;
        ~ImguiRenderer() override = default;

        virtual bool initialize(const ImguiRendererDesc& rendererDesc);
        virtual void destroy() = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual void render(RHI::CommandBuffer* cmdBuffer) = 0;

        virtual void drawTexture(const RHI::TextureView* textureView, const FVector2f& uv0, const FVector2f& uv1) = 0;

        ImGuiContext* getContext() const { return m_Context; }
    protected:
        ImGuiContext* m_Context = nullptr;
    };

    ImguiRenderer* createImguiRenderer(const ImguiRendererDesc& rendererDesc);
    ImguiRenderer* createImguiRenderer(Window* window, RHI::Device* device);
}