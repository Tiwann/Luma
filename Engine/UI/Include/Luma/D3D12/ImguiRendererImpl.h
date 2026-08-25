#pragma once
#include "Luma/Rendering/ImguiRenderer.h"

namespace Luma::D3D12
{
    class FImguiRendererImpl : public IImguiRenderer
    {
    public:
        bool initialize(const FImguiRendererDesc& rendererDesc) override;
        void destroy() override;
        void beginFrame() override;
        void endFrame() override;
        void render(ICommandBuffer* cmdBuffer) override;
        void drawTexture(const ITextureView* textureView, const FVector2f& uv0, const FVector2f& uv1) override;

    private:

    };


}
