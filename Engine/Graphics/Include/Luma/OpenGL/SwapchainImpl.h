#pragma once
#include "Rendering/Swapchain.h"
#include "Common.h"

namespace Luma::OpenGL
{
    class SwapchainImpl final : public Luma::Swapchain
    {
    public:
        bool Initialize(const SwapchainCreateInfo& createInfo) override;
        void Destroy() override;
        bool Recreate() override;
        void SetName(StringView name) override;
        Ref<Luma::Texture> GetTexture(uint32_t index) override;
        Ref<Luma::Texture> GetCurrentTexture() override;

        bool AcquireNextImage(uint32_t& nextImage);
    private:
        uint32_t m_Textures[3] { INVALID_HANDLE<uint32_t>, INVALID_HANDLE<uint32_t>, INVALID_HANDLE<uint32_t> };
    };
}
