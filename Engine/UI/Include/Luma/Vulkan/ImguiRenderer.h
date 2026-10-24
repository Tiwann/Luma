#pragma once
#include "Luma/Containers/HashMap.h"
#include "Luma/Rendering/ImguiRenderer.h"


namespace Luma::Vulkan
{
    class ImguiRenderer : public Luma::ImguiRenderer
    {
    public:
        ImguiRenderer(Window* window, Device* device);
        ~ImguiRenderer() override;

        void beginFrame() override;
        void endFrame() override;
        void render(CommandBuffer* cmdBuffer) override;
        void drawTexture(const TextureView* textureView, const FVector2f& uv0, const FVector2f& uv1) override;

        uint64_t addTexture(const TextureView* texture);
        uint64_t getOrAddTexture(const TextureView* texture);

    private:
        Device* m_Device = nullptr;
        HashMap<const TextureView*, uint64_t> m_Textures;
        Sampler* m_Sampler = nullptr;
    };
}
