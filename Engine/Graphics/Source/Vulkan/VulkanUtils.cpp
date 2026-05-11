#include "Luma/Vulkan/VulkanUtils.h"
#include "Luma/Vulkan/BufferImpl.h"
#include "Luma/Vulkan/Conversions.h"
#include "Luma/Vulkan/RenderDeviceImpl.h"
#include "Luma/Runtime/DebugLevel.h"

namespace Luma::Vulkan
{
    void setVulkanObjectDebugName(const FRenderDeviceImpl* device, VkObjectType objectType, void* handle, FStringView name)
    {
        if constexpr (getDebugLevel() < DebugLevel::Release)
        {
            if (!device) return;
            const VkDevice deviceHandle = device->getHandle();
            VkDebugUtilsObjectNameInfoEXT info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT};
            info.objectHandle = reinterpret_cast<uint64_t>(handle);
            info.objectType = objectType;
            info.pObjectName = *name;
            vkSetDebugUtilsObjectNameEXT(deviceHandle, &info);
        }
    }

    VkPipelineStageFlags getSourcePipelineStageFlags(FResourceAccessFlags accessFlags)
    {
        VkPipelineStageFlags flags = 0;

        if (accessFlags & EResourceAccessBits::ShaderRead ||
            accessFlags & EResourceAccessBits::ShaderWrite)
        {
            flags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT |
                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        }

        if (accessFlags & EResourceAccessBits::ColorAttachmentRead ||
            accessFlags & EResourceAccessBits::ColorAttachmentWrite)
        {
            flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }

        if (accessFlags & EResourceAccessBits::DepthStencilAttachmentRead ||
            accessFlags & EResourceAccessBits::DepthStencilAttachmentWrite)
        {
            flags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        }

        if (accessFlags & EResourceAccessBits::TransferRead ||
            accessFlags & EResourceAccessBits::TransferWrite)
        {
            flags |= VK_PIPELINE_STAGE_TRANSFER_BIT;
        }

        if (accessFlags & EResourceAccessBits::HostRead ||
            accessFlags & EResourceAccessBits::HostWrite)
        {
            flags |= VK_PIPELINE_STAGE_HOST_BIT;
        }

        if (flags == 0)
            flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

        return flags;
    }

    VkPipelineStageFlags getDestPipelineStageFlags(FResourceAccessFlags accessFlags)
    {
        VkPipelineStageFlags flags = 0;

        if (accessFlags & EResourceAccessBits::ShaderRead ||
            accessFlags & EResourceAccessBits::ShaderWrite)
        {
            flags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT |
                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        }

        if (accessFlags & EResourceAccessBits::ColorAttachmentRead ||
            accessFlags & EResourceAccessBits::ColorAttachmentWrite)
        {
            flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }

        if (accessFlags & EResourceAccessBits::DepthStencilAttachmentRead ||
            accessFlags & EResourceAccessBits::DepthStencilAttachmentWrite)
        {
            flags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        }

        if (accessFlags & EResourceAccessBits::TransferRead ||
            accessFlags & EResourceAccessBits::TransferWrite)
        {
            flags |= VK_PIPELINE_STAGE_TRANSFER_BIT;
        }

        if (accessFlags & EResourceAccessBits::HostRead ||
            accessFlags & EResourceAccessBits::HostWrite)
        {
            flags |= VK_PIPELINE_STAGE_HOST_BIT;
        }

        if (flags == 0)
            flags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

        return flags;
    }

    VkImageMemoryBarrier makeTextureBarrier(const FTextureBarrier& barrier)
    {
        const FTextureImpl* texture = static_cast<FTextureImpl*>(barrier.texture);

        VkImageMemoryBarrier vkBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
        vkBarrier.image = texture->getImage();
        vkBarrier.oldLayout = convert<VkImageLayout>(texture->getResourceState());
        vkBarrier.newLayout = convert<VkImageLayout>(barrier.destState);
        vkBarrier.srcAccessMask = convert<VkAccessFlags>(barrier.sourceAccess);
        vkBarrier.dstAccessMask = convert<VkAccessFlags>(barrier.destAccess);
        vkBarrier.subresourceRange.aspectMask = convert(texture->getFormat());
        vkBarrier.subresourceRange.baseMipLevel = 0;
        vkBarrier.subresourceRange.levelCount = texture->getMipCount();
        vkBarrier.subresourceRange.baseArrayLayer = 0;
        vkBarrier.subresourceRange.layerCount = texture->getArrayCount();

        if (barrier.sourceQueue && barrier.destQueue && barrier.sourceQueue != barrier.destQueue)
        {
            const FQueueImpl* srcQueue = static_cast<const FQueueImpl*>(barrier.sourceQueue);
            const FQueueImpl* destQueue = static_cast<const FQueueImpl*>(barrier.destQueue);
            vkBarrier.srcQueueFamilyIndex = srcQueue->getIndex();
            vkBarrier.dstQueueFamilyIndex = destQueue->getIndex();
        } else
        {
            vkBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            vkBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        }

        return vkBarrier;
    }

    VkBufferMemoryBarrier makeBufferBarrier(const FBufferBarrier& barrier)
    {
        const FBufferImpl* buffer = static_cast<FBufferImpl*>(barrier.buffer);
        VkBufferMemoryBarrier vkBarrier = { VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER };
        vkBarrier.buffer = buffer->getHandle();
        vkBarrier.offset = barrier.offset;
        vkBarrier.size = barrier.size;
        vkBarrier.srcAccessMask = convert<VkAccessFlags>(barrier.sourceAccess);
        vkBarrier.dstAccessMask = convert<VkAccessFlags>(barrier.destAccess);
        if (barrier.sourceQueue && barrier.destQueue && barrier.sourceQueue != barrier.destQueue)
        {
            const FQueueImpl* srcQueue = static_cast<const FQueueImpl*>(barrier.sourceQueue);
            const FQueueImpl* destQueue = static_cast<const FQueueImpl*>(barrier.destQueue);
            vkBarrier.srcQueueFamilyIndex = srcQueue->getIndex();
            vkBarrier.dstQueueFamilyIndex = destQueue->getIndex();
        } else
        {
            vkBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            vkBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        }
        return vkBarrier;
    }
}
