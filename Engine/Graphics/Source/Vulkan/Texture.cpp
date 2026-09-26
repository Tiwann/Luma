#include "Luma/Vulkan/Texture.h"
#include "Luma/Vulkan/Device.h"
#include "Luma/Rendering/TextureAspect.h"
#include "Luma/Vulkan/Conversions.h"
#include "Luma/Vulkan/VulkanUtils.h"
#include "Luma/Memory/Ref.h"

#include <vk_mem_alloc.h>


namespace Luma::Vulkan
{
    static TextureDimension getDimension(const uint32_t width, const uint32_t height, const uint32_t depth)
    {
        TextureDimension dimension = TextureDimension::Dim1D;
        if (height > 1)  dimension = TextureDimension::Dim2D;
        if (depth > 1)  dimension = TextureDimension::Dim3D;
        return dimension;
    }

    bool Texture::initialize(const RHI::TextureDesc& textureDesc)
    {
        if (textureDesc.format == Format::None) return false;
        if (textureDesc.sampleCount <= 0) return false;
        if (textureDesc.mipCount <= 0) return false;
        if (textureDesc.width <= 0 || textureDesc.height <= 0) return false;
        if (textureDesc.arrayCount <= 0) return false;
        if (textureDesc.depth == 0) return false;

        Device* device = static_cast<Device*>(textureDesc.device);
        const VmaAllocator allocatorHandle = device->getAllocator();
        vmaDestroyImage(allocatorHandle, m_Image, m_Allocation);

        const TextureDimension dimension =  Vulkan::getDimension(textureDesc.width, textureDesc.height, textureDesc.depth);

        VkImageCreateInfo imageCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
        imageCreateInfo.imageType = convert<VkImageType>(dimension);
        imageCreateInfo.format = convert<VkFormat>(textureDesc.format);
        imageCreateInfo.extent.width = textureDesc.width;
        imageCreateInfo.extent.height = textureDesc.height;
        imageCreateInfo.extent.depth = textureDesc.depth;
        imageCreateInfo.mipLevels = textureDesc.mipCount;
        imageCreateInfo.arrayLayers = textureDesc.arrayCount;
        imageCreateInfo.samples = (VkSampleCountFlagBits)textureDesc.sampleCount;
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.usage = convert<VkImageUsageFlags>(textureDesc.usageFlags);
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        VmaAllocationCreateInfo allocationCreateInfo = { };
        allocationCreateInfo.priority = 1.0f;
        allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

        const VkResult result = vmaCreateImage(allocatorHandle,
           &imageCreateInfo,
           &allocationCreateInfo,
           &m_Image,
           &m_Allocation,
           nullptr);
        if (result != VK_SUCCESS)
            return false;

        const auto& usageFlags = textureDesc.usageFlags;

        TextureAspectFlags aspectFlags = 0;
        if (usageFlags & TextureUsage::Color)
            aspectFlags |= TextureAspect::Color;
        if (usageFlags & TextureUsage::DepthStencil)
            aspectFlags |= TextureAspect::Depth | TextureAspect::Stencil;

        m_Device = device;
        m_Format = textureDesc.format;
        m_Width = textureDesc.width;
        m_Height = textureDesc.height;
        m_Depth = textureDesc.depth;
        m_Mips = textureDesc.mipCount;
        m_ArrayCount = textureDesc.arrayCount;
        m_SampleCount = textureDesc.sampleCount;
        m_UsageFlags = textureDesc.usageFlags;
        m_Dimension = dimension;
        m_State = ResourceState::Undefined;

        RHI::TextureViewDesc tvDesc;
        tvDesc.texture = this;
        tvDesc.device = device;
        tvDesc.width = textureDesc.width;
        tvDesc.height = textureDesc.height;
        tvDesc.depth = textureDesc.depth;
        tvDesc.format = textureDesc.format;
        tvDesc.startMipIndex = 0;
        tvDesc.mipCount = textureDesc.mipCount;
        tvDesc.startArrayIndex = 0;
        tvDesc.arrayCount = textureDesc.arrayCount;
        tvDesc.aspectFlags = aspectFlags;
        if (!m_View.initialize(tvDesc))
            return false;


        RHI::ImmediateExecutor executor(device, device->getRenderQueue());
        executor.execute([this](RHI::CommandBuffer* cmdBuffer)
        {
            TextureBarrier barrier;
            barrier.texture = this;
            barrier.sourceAccess = ResourceAccess::None;
            barrier.destAccess = ResourceAccess::None;
            barrier.destState = ResourceState::General;

            cmdBuffer->textureBarriers(barrier);
        });
        return true;
    }

    bool Texture::resize(const uint32_t width, const uint32_t height, const uint32_t depth)
    {
        RHI::TextureDesc desc;
        desc.format = m_Format;
        desc.sampleCount = m_SampleCount;
        desc.usageFlags = m_UsageFlags;
        desc.width = width;
        desc.height = height;
        desc.depth = depth;
        desc.mipCount = m_Mips;
        desc.device = m_Device;
        desc.arrayCount = m_ArrayCount;
        return initialize(desc);
    }

    void Texture::destroy()
    {
        if (!m_Device) return;
        m_View.destroy();
        const VmaAllocator allocatorHandle = m_Device->getAllocator();
        vmaDestroyImage(allocatorHandle, m_Image, m_Allocation);
        m_Device = nullptr;
        m_Image = nullptr;
        m_Allocation = nullptr;
    }

    bool Texture::isValid()
    {
        return m_Device && m_Image && m_Allocation;
    }

    void Texture::setName(const StringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_IMAGE, m_Image, name);
    }

    VkImage Texture::getImage() const
    {
        return m_Image;
    }

    VmaAllocation Texture::getAllocation() const
    {
        return m_Allocation;
    }

    const RHI::TextureView* Texture::getTextureView() const
    {
        return &m_View;
    }
}
