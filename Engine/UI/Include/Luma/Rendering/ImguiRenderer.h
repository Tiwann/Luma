#pragma once
#include "Luma/Memory/RefCounted.h"
#include "Luma/Math/Vector2.h"
#include <cstdint>

struct ImGuiContext;

namespace Luma
{
    struct IWindow;
    struct IRenderDevice;
    struct ICommandBuffer;
    struct ITextureView;

    struct FImguiRendererDesc
    {
        IWindow* window = nullptr;
        IRenderDevice* device = nullptr;
        uint32_t sampleCount = 0;
    };

    struct IImguiRenderer : IRefCounted<IImguiRenderer>
    {
        IImguiRenderer() = default;
        ~IImguiRenderer() override = default;

        virtual bool initialize(const FImguiRendererDesc& rendererDesc);
        virtual void destroy() = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual void render(ICommandBuffer* cmdBuffer) = 0;

        virtual void drawTexture(ITextureView* textureView, const FVector2f& uv0, const FVector2f& uv1) = 0;

        ImGuiContext* getContext() const { return m_Context; }
    protected:
        ImGuiContext* m_Context = nullptr;
    };

    IImguiRenderer* createImguiRenderer(const FImguiRendererDesc& rendererDesc);
}