#include "Luma/Rendering/RenderDevice.h"

#ifdef LUMA_BUILD_WEBGPU
#include "WebGPU/RenderDevice.h"
#endif

#ifdef LUMA_BUILD_VULKAN
#include "Vulkan/RenderDeviceImpl.h"
#endif

namespace luma
{
    IRenderDevice* createRenderDevice(const FRenderDeviceDesc& deviceDesc)
    {
        IRenderDevice* device = nullptr;
        switch (deviceDesc.deviceType)
        {
        case ERenderDeviceType::None:
            return nullptr;
#ifdef LUMA_BUILD_VULKAN
        case ERenderDeviceType::Vulkan:
            device = new vulkan::FRenderDeviceImpl();
            break;
#endif
        default: return nullptr;
        }

        if (device->initialize(deviceDesc))
        {
            delete device;
            return nullptr;
        }
        return device;
    }
}
