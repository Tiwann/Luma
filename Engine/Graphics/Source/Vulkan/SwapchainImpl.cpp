#include "SwapchainImpl.h"
#include "RenderDeviceImpl.h"
#include "CommandBufferImpl.h"
#include "Conversions.h"
#include <Volk/volk.h>
#include "ImmediateExecutorImpl.h"
#include "Luma/Containers/Array.h"

namespace Luma::Vulkan
{
    bool FSwapchainImpl::initialize(const FSwapchainDesc& swapchainDesc)
    {
        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(swapchainDesc.device);
        const VkSurfaceKHR surfaceHandle = device->getSurface();
        const VkDevice deviceHandle = device->getHandle();
        const FQueueImpl* graphicsQueue = static_cast<FQueueImpl*>(device->getRenderQueue());

        VkSwapchainCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
        createInfo.surface = surfaceHandle;
        createInfo.imageFormat = convert<VkFormat>(swapchainDesc.format);
        createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        createInfo.presentMode = convert<VkPresentModeKHR>(swapchainDesc.presentMode);
        createInfo.clipped = true;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.imageExtent.width = swapchainDesc.width;
        createInfo.imageExtent.height = swapchainDesc.height;
        createInfo.minImageCount = (uint32_t)swapchainDesc.buffering;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        createInfo.oldSwapchain = m_Handle ? m_Handle : nullptr;
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 1;
        createInfo.pQueueFamilyIndices = graphicsQueue->getIndexPtr();

        VkSwapchainKHR swapchainHandle = nullptr;
        if (vkCreateSwapchainKHR(deviceHandle, &createInfo, nullptr, &swapchainHandle) != VK_SUCCESS)
            return false;

        vkDestroySwapchainKHR(deviceHandle, m_Handle, nullptr);
        m_Handle = swapchainHandle;

        m_Device = device;
        m_Buffering = swapchainDesc.buffering;
        m_ImageWidth = swapchainDesc.width;
        m_ImageHeight = swapchainDesc.height;
        m_ImageFormat = swapchainDesc.format;
        m_ImagePresentMode = swapchainDesc.presentMode;

        if (vkGetSwapchainImagesKHR(deviceHandle, m_Handle, (uint32_t*)&m_Buffering, m_Images) != VK_SUCCESS)
            return false;

        TArray<VkImageMemoryBarrier2> barriers;
        for (size_t i = 0; i < getImageCount(); i++)
        {
            vkDestroyImageView(deviceHandle, m_ImageViews[i], nullptr);
            VkImageViewCreateInfo ImageViewCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
            ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            ImageViewCreateInfo.format = convert<VkFormat>(m_ImageFormat);
            ImageViewCreateInfo.image = m_Images[i];
            ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            ImageViewCreateInfo.subresourceRange.layerCount = 1;
            ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            ImageViewCreateInfo.subresourceRange.levelCount = 1;
            ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            vkCreateImageView(deviceHandle, &ImageViewCreateInfo, nullptr, &m_ImageViews[i]);

            VkImageMemoryBarrier2 barrier { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2 };
            barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
            barrier.srcAccessMask = VK_ACCESS_2_NONE;
            barrier.dstAccessMask = VK_ACCESS_2_NONE;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = m_Images[i];
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;
            barrier.srcStageMask = VK_PIPELINE_STAGE_2_NONE;
            barrier.dstStageMask = VK_PIPELINE_STAGE_2_NONE;
            barriers.add(barrier);
        }

        FImmediateExecutorImpl& executor = device->getExecutor();
        executor.execute([barriers](const ICommandBuffer* cmdBuffer)
        {
            VkDependencyInfo inDependency = {VK_STRUCTURE_TYPE_DEPENDENCY_INFO};
            inDependency.dependencyFlags = 0;
            inDependency.imageMemoryBarrierCount = barriers.count();
            inDependency.pImageMemoryBarriers = barriers.data();
            vkCmdPipelineBarrier2(static_cast<const FCommandBufferImpl*>(cmdBuffer)->getHandle(), &inDependency);
        });

        m_Valid = true;
        return true;
    }

    void FSwapchainImpl::destroy()
    {
        const FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(m_Device);
        const VkDevice deviceHandle = device->getHandle();

        for (size_t i = 0; i < getImageCount(); i++)
        {
            vkDestroyImageView(deviceHandle, m_ImageViews[i], nullptr);
            m_ImageViews[i] = nullptr;
        }

        vkDestroySwapchainKHR(deviceHandle, m_Handle, nullptr);
        m_Handle = nullptr;
    }
    
    bool FSwapchainImpl::acquireNextImage(ISemaphore* semaphore, IFence* fence, uint32_t& frameIndex) const
    {
        if (!semaphore) return false;

        const VkFence vkFence = fence ? static_cast<FFenceImpl*>(fence)->getHandle() : nullptr;
        const VkSemaphore vkSem = semaphore ? static_cast<FSemaphoreImpl*>(semaphore)->getHandle() : nullptr;

        const FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(m_Device);
        const VkDevice deviceHandle = device->getHandle();
        const VkResult result = vkAcquireNextImageKHR(deviceHandle, m_Handle, 1'000'000'000, vkSem, vkFence, &frameIndex);
        if (VK_FAILED(result)) return false;
        return true;
    }
    
    VkSwapchainKHR FSwapchainImpl::getHandle() const
    {
        return m_Handle;
    }

    const VkSwapchainKHR* FSwapchainImpl::getHandlePtr() const
    {
        return &m_Handle;
    }

    VkImage FSwapchainImpl::getImage(const uint32_t index) const
    {
        return m_Images[index];
    }

    VkImageView FSwapchainImpl::getImageView(const uint32_t index) const
    {
        return m_ImageViews[index];
    }

    bool FSwapchainImpl::isValid() const
    {
        return ISwapchain::isValid();
    }

    const ITexture* FSwapchainImpl::getTexture()
    {
        if (!m_Device) return nullptr;

        const uint32_t frameIndex = m_Device->getCurrentFrameIndex();
        FTextureImpl& texture = m_Textures[frameIndex];
        texture.m_Device = static_cast<FRenderDeviceImpl*>(m_Device);
        texture.m_Image = m_Images[frameIndex];
        texture.m_Format = m_ImageFormat;
        texture.m_Width = m_ImageWidth;
        texture.m_Height = m_ImageHeight;
        texture.m_Depth = 1;
        texture.m_Dimension = ETextureDimension::Dim2D;
        texture.m_Mips = 1;
        texture.m_SampleCount = 1;
        texture.m_ArrayCount = 1;
        texture.m_UsageFlags = ETextureUsageBits::ColorAttachment;
        texture.m_Allocation = nullptr;
        return &texture;
    }

    const ITextureView* FSwapchainImpl::getTextureView()
    {
        if (!m_Device) return nullptr;
        const ITexture* texture = getTexture();
        if (!texture) return nullptr;

        const uint32_t frameIndex = m_Device->getCurrentFrameIndex();
        FTextureViewImpl& view = m_TextureViews[frameIndex];
        view.m_Device = static_cast<FRenderDeviceImpl*>(m_Device);
        view.m_Handle = m_ImageViews[frameIndex];
        view.m_Format = m_ImageFormat;
        view.m_Width = m_ImageWidth;
        view.m_Height = m_ImageHeight;
        view.m_Depth = 1;
        view.m_BaseMipLevel = 0;
        view.m_MipCount = 1;
        view.m_AspectFlags = ETextureAspectBits::Color;
        view.m_Texture = texture;
        return &view;
    }
}
