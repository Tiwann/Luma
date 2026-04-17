#pragma once
#include "TextureImpl.h"
#include "TextureViewImpl.h"
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Swapchain.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class LUMA_GRAPHICS_API FSwapchainImpl final : public ISwapchain
    {
    public:
        bool initialize(const FSwapchainDesc& swapchainDesc) override;
        void destroy() override;
        bool acquireNextImage(VkSemaphore semaphore, VkFence fence, uint32_t& frameIndex) const;

        VkSwapchainKHR getHandle() const;
        const VkSwapchainKHR* getHandlePtr() const;
        VkImage getImage(uint32_t index) const;
        VkImageView getImageView(uint32_t index) const;

        bool isValid() const override;
        const ITexture* getTexture() override;
        const ITextureView* getTextureView() override;
    private:
        VkSwapchainKHR m_Handle = nullptr;
        VkImage m_Images[3] = { nullptr, nullptr, nullptr };
        VkImageView m_ImageViews[3] = { nullptr, nullptr, nullptr };
        FTextureImpl m_Textures[3];
        FTextureViewImpl m_TextureViews[3];
    };
}