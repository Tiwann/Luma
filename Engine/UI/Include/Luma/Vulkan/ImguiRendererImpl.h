#pragma once
#include "Luma/Containers/HashMap.h"
#include "Luma/Rendering/ImguiRenderer.h"


namespace Luma
{
    struct ISampler;
}

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class FImguiRendererImpl : public IImguiRenderer
    {
    public:
        bool initialize(const FImguiRendererDesc& rendererDesc) override;
        void destroy() override;
        void beginFrame() override;
        void endFrame() override;
        void render(ICommandBuffer* cmdBuffer) override;
        void drawTexture(ITextureView* textureView, const FVector2f& uv0, const FVector2f& uv1) override;

        uint64_t addTexture(const ITextureView* texture);
        uint64_t getOrAddTexture(const ITextureView* texture);

    private:
        FRenderDeviceImpl* m_Device = nullptr;
        THashMap<const ITextureView*, uint64_t> m_Textures;
        ISampler* m_Sampler = nullptr;
    };
}
