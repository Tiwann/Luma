#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Rendering/ResourceBarrier.h"
#include <volk.h>


namespace Luma::Vulkan
{
    class FQueueImpl;

    void setVulkanObjectDebugName(const class FRenderDeviceImpl* device, VkObjectType objectType, void* handle, FStringView name);
    VkPipelineStageFlags2 getSourcePipelineStageFlags(FResourceAccessFlags accessFlags);
    VkPipelineStageFlags2 getDestPipelineStageFlags(FResourceAccessFlags accessFlags);
    VkImageMemoryBarrier2 makeTextureBarrier(const FTextureBarrier& barrier);
    VkBufferMemoryBarrier2 makeBufferBarrier(const FBufferBarrier& barrier);
    VkSemaphore createSemaphore(VkDevice device);
    VkCommandPool createCommandPool(VkDevice device, const FQueueImpl& queue);
}
