#pragma once
#include "Texture.h"
#include "TextureView.h"
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Swapchain.h"
#include "Luma/Rendering/Constants.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class Device;

    class LUMA_GRAPHICS_API Swapchain final : public RHI::Swapchain
    {
    public:
        bool initialize(const RHI::SwapchainDesc& swapchainDesc) override;
        void destroy() override;
        bool acquireNextTexture(uint32_t& textureIndex, VkSemaphore textureAvailableSemaphore);

        VkSwapchainKHR getHandle() const;
        const VkSwapchainKHR* getHandlePtr() const;
        VkImage getImage(uint32_t index) const;
        VkImageView getImageView(uint32_t index) const;

        bool isValid() const override;
        RHI::Texture* getTexture(uint32_t index) override;
        RHI::TextureView* getTextureView(uint32_t index) override;

        void setName(StringView name) override;

    private:
        VkSwapchainKHR m_Handle = nullptr;
        VkImage m_Images[RHI::MAX_SWAPCHAIN_IMAGES] = { nullptr };
        VkImageView m_ImageViews[RHI::MAX_SWAPCHAIN_IMAGES] = { nullptr };
        Texture m_Textures[RHI::MAX_SWAPCHAIN_IMAGES];
        TextureView m_TextureViews[RHI::MAX_SWAPCHAIN_IMAGES];
    };
}
