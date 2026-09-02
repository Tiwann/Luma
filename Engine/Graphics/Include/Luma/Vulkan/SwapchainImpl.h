#pragma once
#include "TextureImpl.h"
#include "TextureViewImpl.h"
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Swapchain.h"
#include "Luma/Rendering/Constants.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;
    class FSemaphoreImpl;

    class LUMA_GRAPHICS_API FSwapchainImpl final : public ISwapchain
    {
    public:
        bool initialize(const FSwapchainDesc& swapchainDesc) override;
        void destroy() override;
        bool acquireNextTexture(uint32_t& textureIndex, VkSemaphore textureAvailableSemaphore);

        VkSwapchainKHR getHandle() const;
        const VkSwapchainKHR* getHandlePtr() const;
        VkImage getImage(uint32_t index) const;
        VkImageView getImageView(uint32_t index) const;

        bool isValid() const override;
        ITexture* getTexture(uint32_t index) override;
        ITextureView* getTextureView(uint32_t index) override;

        void setName(FStringView name) override;

    private:
        VkSwapchainKHR m_Handle = nullptr;
        VkImage m_Images[MAX_SWAPCHAIN_IMAGES] = { nullptr };
        VkImageView m_ImageViews[MAX_SWAPCHAIN_IMAGES] = { nullptr };
        FTextureImpl m_Textures[MAX_SWAPCHAIN_IMAGES];
        FTextureViewImpl m_TextureViews[MAX_SWAPCHAIN_IMAGES];
    };
}
