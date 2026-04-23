#include "TextureViewImpl.h"
#include "TextureImpl.h"
#include "RenderDeviceImpl.h"
#include "Conversions.h"
#include "VulkanUtils.h"

#include <Volk/volk.h>


namespace Luma::Vulkan
{
    bool FTextureViewImpl::initialize(const FTextureViewDesc& textureViewDesc)
    {
        if (!textureViewDesc.device) return false;

        VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_1D;
        if (textureViewDesc.height > 1) viewType = VK_IMAGE_VIEW_TYPE_2D;
        if (textureViewDesc.depth > 1) viewType = VK_IMAGE_VIEW_TYPE_3D;

        const FTextureImpl* texture = dynamic_cast<const FTextureImpl*>(textureViewDesc.texture);
        if (!texture) return false;

        VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
        imageViewCreateInfo.image = texture->getImage();
        imageViewCreateInfo.viewType = viewType;
        imageViewCreateInfo.format = convert<VkFormat>(textureViewDesc.format);
        imageViewCreateInfo.subresourceRange.aspectMask = convert<VkImageAspectFlags>(textureViewDesc.aspectFlags);
        imageViewCreateInfo.subresourceRange.baseMipLevel = textureViewDesc.baseMipLevel;
        imageViewCreateInfo.subresourceRange.levelCount = textureViewDesc.mipCount;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        imageViewCreateInfo.components = convert<VkComponentMapping>(textureViewDesc.mapping);

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(textureViewDesc.device);
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
        m_BaseMipLevel = textureViewDesc.baseMipLevel;
        m_MipCount = textureViewDesc.mipCount;
        return true;
    }

    void FTextureViewImpl::destroy()
    {
        if (!m_Device) return;
        const VkDevice deviceHandle = m_Device->getHandle();
        vkDestroyImageView(deviceHandle, m_Handle, nullptr);
        m_Handle = nullptr;
    }

    void FTextureViewImpl::setName(FStringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_IMAGE_VIEW, m_Handle, name);
    }

    VkImage FTextureViewImpl::getImage() const
    {
        if (!m_Texture) return nullptr;
        const FTextureViewImpl* texture = dynamic_cast<const FTextureViewImpl*>(m_Texture);
        if (!texture) return nullptr;
        return texture->getImage();
    }

    VkImageView FTextureViewImpl::getHandle() const
    {
        return m_Handle;
    }
}
