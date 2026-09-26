#include "Luma/Vulkan/TextureView.h"
#include "Luma/Vulkan/Texture.h"
#include "Luma/Vulkan/Device.h"
#include "Luma/Vulkan/Conversions.h"
#include "Luma/Vulkan/VulkanUtils.h"

#include <volk.h>

namespace Luma::Vulkan
{
    bool TextureView::initialize(const RHI::TextureViewDesc& textureViewDesc)
    {
        if (!textureViewDesc.device) return false;

        VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_1D;
        if (textureViewDesc.height > 1) viewType = VK_IMAGE_VIEW_TYPE_2D;
        if (textureViewDesc.depth > 1) viewType = VK_IMAGE_VIEW_TYPE_3D;

        const Texture* texture = dynamic_cast<const Texture*>(textureViewDesc.texture);
        if (!texture) return false;

        VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
        imageViewCreateInfo.image = texture->getImage();
        imageViewCreateInfo.viewType = viewType;
        imageViewCreateInfo.format = convert<VkFormat>(textureViewDesc.format);
        imageViewCreateInfo.subresourceRange.aspectMask = convert<VkImageAspectFlags>(textureViewDesc.aspectFlags);
        imageViewCreateInfo.subresourceRange.baseMipLevel = textureViewDesc.startMipIndex;
        imageViewCreateInfo.subresourceRange.levelCount = textureViewDesc.mipCount;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = textureViewDesc.startArrayIndex;
        imageViewCreateInfo.subresourceRange.layerCount = textureViewDesc.arrayCount;
        imageViewCreateInfo.components = convert<VkComponentMapping>(textureViewDesc.mapping);

        Device* device = static_cast<Device*>(textureViewDesc.device);
        const VkDevice deviceHandle = device->getHandle();
        vkDestroyImageView(deviceHandle, m_Handle, nullptr);
        if (vkCreateImageView(deviceHandle, &imageViewCreateInfo, nullptr, &m_Handle) != VK_SUCCESS)
            return false;

        m_Device = device;
        m_Texture = textureViewDesc.texture;
        m_Format = textureViewDesc.format;
        m_AspectFlags = textureViewDesc.aspectFlags;
        m_Width = textureViewDesc.width;
        m_Height = textureViewDesc.height;
        m_Depth = textureViewDesc.depth;
        m_StartMipIndex = textureViewDesc.startMipIndex;
        m_MipCount = textureViewDesc.mipCount;
        m_StartArrayIndex = textureViewDesc.startMipIndex;
        m_ArrayCount = textureViewDesc.arrayCount;
        return true;
    }

    void TextureView::destroy()
    {
        if (!m_Device) return;
        const VkDevice deviceHandle = m_Device->getHandle();
        vkDestroyImageView(deviceHandle, m_Handle, nullptr);
        m_Handle = nullptr;
    }

    void TextureView::setName(StringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_IMAGE_VIEW, m_Handle, name);
    }

    VkImage TextureView::getImage() const
    {
        if (!m_Texture) return nullptr;
        const TextureView* texture = dynamic_cast<const TextureView*>(m_Texture);
        if (!texture) return nullptr;
        return texture->getImage();
    }

    VkImageView TextureView::getHandle() const
    {
        return m_Handle;
    }
}
