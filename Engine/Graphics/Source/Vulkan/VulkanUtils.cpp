#include "VulkanUtils.h"
#include "RenderDeviceImpl.h"
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
}
