#pragma once
#include "Luma/Containers/HashMap.h"
#include "Luma/Rendering/ImguiRenderer.h"
#include "Luma/Vulkan/Sampler.h"

namespace Luma::Vulkan
{
    class ImguiRenderer : public Luma::ImguiRenderer
    {
    public:
        ImguiRenderer(Ref<Window> window, Ref<RHI::Device> device);
        ~ImguiRenderer() override;

        void beginFrame() override;
        void endFrame() override;
        Ref<RHI::Texture> render() override;
        void drawTexture(const RHI::TextureView* textureView, const FVector2f& uv0, const FVector2f& uv1) override;

        uint64_t addTexture(const RHI::TextureView* texture);
        uint64_t getOrAddTexture(const RHI::TextureView* texture);

    private:
        HashMap<const RHI::TextureView*, uint64_t> m_Textures;
        Ref<Vulkan::Sampler> m_Sampler = nullptr;
    };
}
