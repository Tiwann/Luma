#include "Luma/Vulkan/TextureImpl.h"
#include "Luma/Vulkan/RenderDeviceImpl.h"
#include "Luma/Rendering/TextureAspect.h"
#include "Luma/Vulkan/Conversions.h"
#include "Luma/Vulkan/VulkanUtils.h"
#include "Luma/Memory/Ref.h"

#include <vma/vk_mem_alloc.h>


namespace Luma::Vulkan
{
    static ETextureDimension getDimension(const uint32_t width, const uint32_t height, const uint32_t depth)
    {
        ETextureDimension dimension = ETextureDimension::Dim1D;
        if (height > 1)  dimension = ETextureDimension::Dim2D;
        if (depth > 1)  dimension = ETextureDimension::Dim3D;
        return dimension;
    }

    bool FTextureImpl::initialize(const FTextureDesc& textureDesc)
    {
        if (textureDesc.format == EFormat::None) return false;
        if (textureDesc.sampleCount <= 0) return false;
        if (textureDesc.mipCount <= 0) return false;
        if (textureDesc.width <= 0 || textureDesc.height <= 0) return false;
        if (textureDesc.arrayCount <= 0) return false;
        if (textureDesc.depth == 0) return false;

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(textureDesc.device);
        const VmaAllocator allocatorHandle = device->getAllocator();
        vmaDestroyImage(allocatorHandle, m_Image, m_Allocation);

        const ETextureDimension dimension =  Vulkan::getDimension(textureDesc.width, textureDesc.height, textureDesc.depth);

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
        const bool isColorAttachment = usageFlags & ETextureUsageBits::ColorAttachment;
        const bool isDepthAttachment = usageFlags & ETextureUsageBits::DepthStencilAttachment;
        const bool isSampled = usageFlags & ETextureUsageBits::Sampled;

        FTextureAspectFlags aspectFlags = 0;
        if (isColorAttachment || isSampled) aspectFlags |= ETextureAspectBits::Color;
        if (isDepthAttachment)
        {
            aspectFlags |= ETextureAspectBits::Depth;
            aspectFlags |= ETextureAspectBits::Stencil;
        }

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

        FTextureViewDesc tvDesc;
        tvDesc.texture = this;
        tvDesc.device = device;
        tvDesc.width = textureDesc.width;
        tvDesc.height = textureDesc.height;
        tvDesc.depth = textureDesc.depth;
        tvDesc.format = textureDesc.format;
        tvDesc.baseMipLevel = 0;
        tvDesc.mipCount = textureDesc.mipCount;
        tvDesc.baseArray = 0;
        tvDesc.arrayCount = textureDesc.arrayCount;
        tvDesc.aspectFlags = aspectFlags;
        if (!m_View.initialize(tvDesc))
            return false;

        // DECIDED TO EXPLICITLY TRANSITION TO LAYOUT GENERAL BY DEFAULT
        const EResourceAccessBits destAccess = isColorAttachment ? EResourceAccessBits::ColorAttachmentWrite :
        isDepthAttachment ? EResourceAccessBits::DepthStencilAttachmentWrite :
        isSampled ? EResourceAccessBits::ShaderRead : EResourceAccessBits::None;

        const EResourceState destState = isColorAttachment ? EResourceState::ColorAttachment :
        isDepthAttachment ? EResourceState::DepthStencilAttachment :
        isSampled ? EResourceState::ShaderRead : EResourceState::General;

        FTextureBarrier barrier;
        barrier.texture = this;
        barrier.sourceAccess = EResourceAccessBits::None;
        barrier.destAccess = destAccess;
        barrier.destState = destState;
        barrier.destQueue = nullptr;
        barrier.destQueue = nullptr;

        Ref<ICommandBuffer> cmdBuffer = device->createRenderCommandBuffer();
        Ref<IFence> fence = device->createFence(FFenceDesc());

        if (cmdBuffer->begin())
        {
            cmdBuffer->textureBarrier(barrier);
            cmdBuffer->end();

            FQueueImpl* queue = static_cast<FQueueImpl*>(device->getRenderQueue());
            if (!queue) return false;
            queue->executeCommandBuffer(cmdBuffer, fence);
            fence->wait(FENCE_WAIT_INFINITE);
            return true;
        }
        return false;
    }

    bool FTextureImpl::resize(const uint32_t width, const uint32_t height, const uint32_t depth)
    {
        FTextureDesc desc;
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

    void FTextureImpl::destroy()
    {
        if (!m_Device) return;
        m_View.destroy();
        const VmaAllocator allocatorHandle = m_Device->getAllocator();
        vmaDestroyImage(allocatorHandle, m_Image, m_Allocation);
        m_Device = nullptr;
        m_Image = nullptr;
    }

    bool FTextureImpl::isValid()
    {
        return m_Device && m_Image && m_Allocation;
    }

    void FTextureImpl::setName(const FStringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_IMAGE, m_Image, name);
    }

    VkImage FTextureImpl::getImage() const
    {
        return m_Image;
    }

    VmaAllocation FTextureImpl::getAllocation() const
    {
        return m_Allocation;
    }

    const ITextureView* FTextureImpl::getTextureView() const
    {
        return &m_View;
    }
}
