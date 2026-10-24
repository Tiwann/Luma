#pragma once
#include "Luma/Math/Vector2.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/Forward.h"

struct ImGuiContext;

namespace Luma
{
    struct Window;

    struct ImguiRenderer
    {
        ImguiRenderer();
        virtual ~ImguiRenderer() = default;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual void render(CommandBuffer* cmdBuffer) = 0;

        virtual void drawTexture(const TextureView* textureView, const FVector2f& uv0, const FVector2f& uv1) = 0;

        ImGuiContext* getContext() const { return m_Context; }


    protected:
        ImGuiContext* m_Context = nullptr;
        Ref<Texture> m_Texture = nullptr;
    };

    ImguiRenderer* createImguiRenderer(Window* window, Device* device);
}