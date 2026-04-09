#include "Luma/Rendering/RenderDevice.h"

#ifdef LUMA_BUILD_WEBGPU
#include "WebGPU/RenderDevice.h"
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
#ifdef LUMA_BUILD_WEBGPU
        case ERenderDeviceType::WebGPU:
            device = new WebGPU::FRenderDevice();
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
