#pragma once
#include "Luma/Math/Vector2.h"
#include "Luma/Memory/Ref.h"

struct ImGuiContext;

namespace Luma
{
    namespace RHI
    {
        struct Fence;
        struct CommandBuffer;
        struct TextureView;
        struct Texture;
        struct Device;
    }

    struct Window;

    struct ImguiRenderer
    {
        ImguiRenderer(Ref<Window> window, Ref<RHI::Device> device);
        virtual ~ImguiRenderer() = default;
        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual Ref<RHI::Texture> render() = 0;
        virtual void drawTexture(const RHI::TextureView* textureView, const FVector2f& uv0, const FVector2f& uv1) = 0;

        ImGuiContext* getContext() const { return m_Context; }

    protected:
        ImGuiContext* m_Context = nullptr;
        Ref<Window> m_Window = nullptr;
        Ref<RHI::Device> m_Device = nullptr;
        Ref<RHI::Texture> m_Texture = nullptr;
        Ref<RHI::CommandBuffer> m_CmdBuff = nullptr;
        Ref<RHI::Fence> m_Fence = nullptr;
    };

    ImguiRenderer* createImguiRenderer(Ref<Window> window, Ref<RHI::Device> device);
}