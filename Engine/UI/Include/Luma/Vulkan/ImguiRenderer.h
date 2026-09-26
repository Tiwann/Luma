#pragma once
#include "Luma/Containers/HashMap.h"
#include "Luma/Rendering/ImguiRenderer.h"


namespace Luma
{

    namespace RHI
    {
    struct Sampler;

    }
}

namespace Luma::Vulkan
{
    class Device;

    class ImguiRenderer : public Luma::ImguiRenderer
    {
    public:
        bool initialize(const ImguiRendererDesc& rendererDesc) override;
        void destroy() override;
        void beginFrame() override;
        void endFrame() override;
        void render(RHI::CommandBuffer* cmdBuffer) override;
        void drawTexture(const RHI::TextureView* textureView, const Vector2f& uv0, const Vector2f& uv1) override;

        uint64_t addTexture(const RHI::TextureView* texture);
        uint64_t getOrAddTexture(const RHI::TextureView* texture);

    private:
        Device* m_Device = nullptr;
        THashMap<const RHI::TextureView*, uint64_t> m_Textures;
        RHI::Sampler* m_Sampler = nullptr;
    };
}
