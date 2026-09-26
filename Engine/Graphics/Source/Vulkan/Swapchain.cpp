#include "Luma/Vulkan/Swapchain.h"
#include "Luma/Vulkan/Device.h"
#include "Luma/Vulkan/Conversions.h"
#include "Luma/Containers/Array.h"
#include "Luma/Vulkan/VulkanUtils.h"
#include "Luma/Containers/StringFormat.h"
#include <volk.h>


namespace Luma::Vulkan
{
    bool Swapchain::initialize(const RHI::SwapchainDesc& swapchainDesc)
    {
        Device* device = static_cast<Device*>(swapchainDesc.device);
        const VkSurfaceKHR surfaceHandle = device->getSurface();
        const VkDevice deviceHandle = device->getHandle();
        const Queue* graphicsQueue = static_cast<Queue*>(device->getRenderQueue());

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
        m_Width = swapchainDesc.width;
        m_Height = swapchainDesc.height;
        m_Format = swapchainDesc.format;
        m_PresentMode = swapchainDesc.presentMode;

        if (vkGetSwapchainImagesKHR(deviceHandle, m_Handle, (uint32_t*)&m_Buffering, m_Images) != VK_SUCCESS)
            return false;

        for (uint32_t i = 0; i < (uint32_t)m_Buffering; i++)
            setVulkanObjectDebugName(static_cast<Device*>(m_Device), VK_OBJECT_TYPE_IMAGE, m_Images[i], strfmt("Swapchain Image [{}]", i));

        for (size_t i = 0; i < getTextureCount(); i++)
        {
            vkDestroyImageView(deviceHandle, m_ImageViews[i], nullptr);

            VkImageViewCreateInfo ImageViewCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
            ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            ImageViewCreateInfo.format = convert<VkFormat>(m_Format);
            ImageViewCreateInfo.image = m_Images[i];
            ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            ImageViewCreateInfo.subresourceRange.layerCount = 1;
            ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            ImageViewCreateInfo.subresourceRange.levelCount = 1;
            ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            vkCreateImageView(deviceHandle, &ImageViewCreateInfo, nullptr, &m_ImageViews[i]);

            m_Textures[i].m_State = ResourceState::Undefined;
        }

        m_Valid = true;
        return true;
    }

    void Swapchain::destroy()
    {
        const Device* device = static_cast<Device*>(m_Device);
        const VkDevice deviceHandle = device->getHandle();

        for (size_t i = 0; i < getTextureCount(); i++)
        {
            vkDestroyImageView(deviceHandle, m_ImageViews[i], nullptr);
            m_ImageViews[i] = nullptr;
            m_Images[i] = nullptr;
        }

        vkDestroySwapchainKHR(deviceHandle, m_Handle, nullptr);
        m_Handle = nullptr;
    }

    bool Swapchain::acquireNextTexture(uint32_t& textureIndex, VkSemaphore textureAvailableSemaphore)
    {
        const Device* device = static_cast<Device*>(m_Device);
        const VkDevice deviceHandle = device->getHandle();
        const VkResult result = vkAcquireNextImageKHR(deviceHandle, m_Handle, 1'000'000'000, textureAvailableSemaphore, nullptr, &textureIndex);
        if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
        {
            if (result == VK_SUBOPTIMAL_KHR)
                m_Valid = false;
            return true;
        }
        return false;
    }

    VkSwapchainKHR Swapchain::getHandle() const
    {
        return m_Handle;
    }

    const VkSwapchainKHR* Swapchain::getHandlePtr() const
    {
        return &m_Handle;
    }

    VkImage Swapchain::getImage(const uint32_t index) const
    {
        return m_Images[index];
    }

    VkImageView Swapchain::getImageView(const uint32_t index) const
    {
        return m_ImageViews[index];
    }

    bool Swapchain::isValid() const
    {
        return m_Valid;
    }

    RHI::Texture* Swapchain::getTexture(uint32_t index)
    {
        if (!m_Device) return nullptr;
        LUMA_ASSERT(index <= getTextureCount(), "Index out of swapchain's image count range!");

        TextureView& view = m_TextureViews[index];

        Texture& texture = m_Textures[index];
        texture.m_Device = static_cast<Device*>(m_Device);
        texture.m_Image = m_Images[index];
        texture.m_Format = m_Format;
        texture.m_Width = m_Width;
        texture.m_Height = m_Height;
        texture.m_Depth = 1;
        texture.m_Dimension = TextureDimension::Dim2D;
        texture.m_Mips = 1;
        texture.m_SampleCount = 1;
        texture.m_ArrayCount = 1;
        texture.m_UsageFlags = TextureUsage::Color;
        texture.m_Allocation = nullptr;
        texture.m_View = view;


        view.m_Device = static_cast<Device*>(m_Device);
        view.m_Handle = m_ImageViews[index];
        view.m_Format = m_Format;
        view.m_Width = m_Width;
        view.m_Height = m_Height;
        view.m_Depth = 1;
        view.m_StartMipIndex = 0;
        view.m_MipCount = 1;
        view.m_AspectFlags = TextureAspect::Color;
        view.m_Texture = &texture;
        return &texture;
    }

    RHI::TextureView* Swapchain::getTextureView(uint32_t index)
    {
        if (!m_Device) return nullptr;
        LUMA_ASSERT(index <= getTextureCount(), "Index out of swapchain's image count range!");

        const RHI::Texture* texture = getTexture(index);
        if (!texture) return nullptr;

        TextureView& view = m_TextureViews[index];
        view.m_Device = static_cast<Device*>(m_Device);
        view.m_Handle = m_ImageViews[index];
        view.m_Format = m_Format;
        view.m_Width = m_Width;
        view.m_Height = m_Height;
        view.m_Depth = 1;
        view.m_StartMipIndex = 0;
        view.m_MipCount = 1;
        view.m_AspectFlags = TextureAspect::Color;
        view.m_Texture = texture;
        return &view;
    }

    void Swapchain::setName(StringView name)
    {
        setVulkanObjectDebugName(static_cast<Device*>(m_Device), VK_OBJECT_TYPE_SWAPCHAIN_KHR, m_Handle, name);
    }
}
