#pragma once
#include "TextureImpl.h"
#include "TextureViewImpl.h"
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Swapchain.h"
#include "VulkanFwd.h"
#include "Luma/Rendering/Fence.h"
#include "Luma/Rendering/Semaphore.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;
    class FSemaphoreImpl;

    class LUMA_GRAPHICS_API FSwapchainImpl final : public ISwapchain
    {
    public:
        bool initialize(const FSwapchainDesc& swapchainDesc) override;
        void destroy() override;
        bool acquireNextImage(FSemaphoreImpl* semaphore, IFence* fence, uint32_t& frameIndex);

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
        VkImage m_Images[3] = { nullptr, nullptr, nullptr };
        VkImageView m_ImageViews[3] = { nullptr, nullptr, nullptr };
        FTextureImpl m_Textures[3];
        FTextureViewImpl m_TextureViews[3];
    };
}
