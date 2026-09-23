#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Rendering/ResourceBarrier.h"
#include <volk.h>


namespace Luma::Vulkan
{
    class Device;
    class Queue;

    void setVulkanObjectDebugName(const Device* device, VkObjectType objectType, void* handle, FStringView name);
    VkPipelineStageFlags2 getSourcePipelineStageFlags(ResourceAccessFlags accessFlags);
    VkPipelineStageFlags2 getDestPipelineStageFlags(ResourceAccessFlags accessFlags);
    VkImageMemoryBarrier2 makeTextureBarrier(const TextureBarrier& barrier);
    VkBufferMemoryBarrier2 makeBufferBarrier(const BufferBarrier& barrier);
    VkSemaphore createSemaphore(VkDevice device);
    VkCommandPool createCommandPool(VkDevice device, const Queue& queue);
}
