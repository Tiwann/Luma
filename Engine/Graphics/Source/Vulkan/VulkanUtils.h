#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Rendering/ResourceBarrier.h"
#include <Volk/volk.h>


namespace Luma::Vulkan
{
    void setVulkanObjectDebugName(const class FRenderDeviceImpl* device, VkObjectType objectType, void* handle, FStringView name);
    VkPipelineStageFlags getSourcePipelineStageFlags(FResourceAccessFlags accessFlags);
    VkPipelineStageFlags getDestPipelineStageFlags(FResourceAccessFlags accessFlags);
    VkImageMemoryBarrier makeTextureBarrier(const FTextureBarrier& barrier);
    VkBufferMemoryBarrier makeBufferBarrier(const FBufferBarrier& barrier);
}
