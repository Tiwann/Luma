#pragma once
#include "Luma/Containers/StringView.h"
#include <Volk/volk.h>


namespace Luma::Vulkan
{
    void setVulkanObjectDebugName(const class FRenderDeviceImpl* device, VkObjectType objectType, void* handle, FStringView name);
}
