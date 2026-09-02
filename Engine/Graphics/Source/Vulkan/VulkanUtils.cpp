#include "Luma/Vulkan/VulkanUtils.h"
#include "Luma/Vulkan/BufferImpl.h"
#include "Luma/Vulkan/Conversions.h"
#include "Luma/Vulkan/RenderDeviceImpl.h"
#include "Luma/Runtime/DebugLevel.h"

namespace Luma::Vulkan
{
    void setVulkanObjectDebugName(const FRenderDeviceImpl* device, VkObjectType objectType, void* handle, FStringView name)
    {
        if constexpr (DEBUG_LEVEL < DebugLevel::Release)
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

    VkPipelineStageFlags2 getSourcePipelineStageFlags(FResourceAccessFlags accessFlags)
    {
        VkPipelineStageFlags2 flags = 0;

        if (accessFlags & EResourceAccessBits::ShaderRead ||
            accessFlags & EResourceAccessBits::ShaderWrite)
        {
            flags |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT |
                VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT |
                VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
        }

        if (accessFlags & EResourceAccessBits::ColorTargetRead ||
            accessFlags & EResourceAccessBits::ColorTargetWrite)
        {
            flags |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        }

        if (accessFlags & EResourceAccessBits::DepthStencilTargetRead ||
            accessFlags & EResourceAccessBits::DepthStencilTargetWrite)
        {
            flags |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT |
                VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
        }

        if (accessFlags & EResourceAccessBits::CopyRead ||
            accessFlags & EResourceAccessBits::CopyWrite)
        {
            flags |= VK_PIPELINE_STAGE_2_TRANSFER_BIT;
        }

        if (accessFlags & EResourceAccessBits::HostRead ||
            accessFlags & EResourceAccessBits::HostWrite)
        {
            flags |= VK_PIPELINE_STAGE_2_HOST_BIT;
        }

        if (flags == 0)
            flags = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;

        return flags;
    }

    VkPipelineStageFlags2 getDestPipelineStageFlags(FResourceAccessFlags accessFlags)
    {
        VkPipelineStageFlags2 flags = 0;

        if (accessFlags & EResourceAccessBits::ShaderRead ||
            accessFlags & EResourceAccessBits::ShaderWrite)
        {
            flags |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT |
                VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT |
                VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
        }

        if (accessFlags & EResourceAccessBits::ColorTargetRead ||
            accessFlags & EResourceAccessBits::ColorTargetWrite)
        {
            flags |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        }

        if (accessFlags & EResourceAccessBits::DepthStencilTargetRead ||
            accessFlags & EResourceAccessBits::DepthStencilTargetWrite)
        {
            flags |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT |
                VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
        }

        if (accessFlags & EResourceAccessBits::CopyRead ||
            accessFlags & EResourceAccessBits::CopyWrite)
        {
            flags |= VK_PIPELINE_STAGE_2_TRANSFER_BIT;
        }

        if (accessFlags & EResourceAccessBits::HostRead ||
            accessFlags & EResourceAccessBits::HostWrite)
        {
            flags |= VK_PIPELINE_STAGE_2_HOST_BIT;
        }

        if (flags == 0)
            flags = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;

        return flags;
    }

    VkImageMemoryBarrier2 makeTextureBarrier(const FTextureBarrier& barrier)
    {
        const FTextureImpl* texture = static_cast<FTextureImpl*>(barrier.texture);

        VkImageMemoryBarrier2 vkBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2 };
        vkBarrier.image = texture->getImage();
        vkBarrier.oldLayout = convert<VkImageLayout>(texture->getResourceState());
        vkBarrier.newLayout = convert<VkImageLayout>(barrier.destState);
        vkBarrier.srcAccessMask = convert<VkAccessFlags2>(barrier.sourceAccess);
        vkBarrier.dstAccessMask = convert<VkAccessFlags2>(barrier.destAccess);
        vkBarrier.subresourceRange.aspectMask = convert(texture->getFormat());
        vkBarrier.subresourceRange.baseMipLevel = 0;
        vkBarrier.subresourceRange.levelCount = texture->getMipCount();
        vkBarrier.subresourceRange.baseArrayLayer = 0;
        vkBarrier.subresourceRange.layerCount = texture->getArrayCount();
        vkBarrier.srcStageMask = getSourcePipelineStageFlags(barrier.sourceAccess);
        vkBarrier.dstStageMask = getDestPipelineStageFlags(barrier.destAccess);

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

    VkBufferMemoryBarrier2 makeBufferBarrier(const FBufferBarrier& barrier)
    {
        const FBufferImpl* buffer = static_cast<FBufferImpl*>(barrier.buffer);

        VkBufferMemoryBarrier2 vkBarrier = { VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER };
        vkBarrier.buffer = buffer->getHandle();
        vkBarrier.offset = barrier.offset;
        vkBarrier.size = barrier.size;
        vkBarrier.srcAccessMask = convert<VkAccessFlags2>(barrier.sourceAccess);
        vkBarrier.dstAccessMask = convert<VkAccessFlags2>(barrier.destAccess);
        vkBarrier.srcStageMask = getSourcePipelineStageFlags(barrier.sourceAccess);
        vkBarrier.dstStageMask = getDestPipelineStageFlags(barrier.destAccess);

        const FQueueImpl* srcQueue = static_cast<const FQueueImpl*>(barrier.sourceQueue);
        const FQueueImpl* destQueue = static_cast<const FQueueImpl*>(barrier.destQueue);

        if (barrier.sourceQueue && barrier.destQueue && !srcQueue->same(*destQueue))
        {
            vkBarrier.srcQueueFamilyIndex = srcQueue->getIndex();
            vkBarrier.dstQueueFamilyIndex = destQueue->getIndex();
        } else
        {
            vkBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            vkBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        }
        return vkBarrier;
    }

    VkCommandPool createCommandPool(VkDevice device, const FQueueImpl& queue)
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
        commandPoolCreateInfo.queueFamilyIndex = queue.getIndex();
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        VkCommandPool result = nullptr;
        vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &result);
        return result;
    }

    VkSemaphore createSemaphore(VkDevice device)
    {
        const VkSemaphoreCreateInfo createInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        VkSemaphore semaphore;
        const VkResult result = vkCreateSemaphore(device, &createInfo, nullptr, &semaphore);
        if (VK_FAILED(result)) return nullptr;
        return semaphore;
    }
}
