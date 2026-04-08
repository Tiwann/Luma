#include "Luma/RHI/RenderDevice.h"
#include "WebGPU/RenderDevice.h"

namespace Luma
{
    IRenderDevice* createRenderDevice(const FRenderDeviceDesc& deviceDesc)
    {
        IRenderDevice* device = nullptr;
        switch (deviceDesc.deviceType)
        {
        case ERenderDeviceType::None:
            break;
        case ERenderDeviceType::WebGPU:
            device = new WebGPU::FRenderDevice();
            break;
        }

        if (device && device->initialize(deviceDesc))
        {
            delete device;
            return nullptr;
        }

        return device;
    }
}
