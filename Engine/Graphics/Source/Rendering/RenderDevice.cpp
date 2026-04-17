#include "Luma/Rendering/RenderDevice.h"

#include "Luma/Rendering/CommandBuffer.h"

#ifdef LUMA_BUILD_WEBGPU
#include "WebGPU/RenderDevice.h"
#endif

#ifdef LUMA_BUILD_VULKAN
#include "Vulkan/RenderDeviceImpl.h"
#endif

namespace Luma
{
    ICommandBuffer* IRenderDevice::createRenderCommandBuffer()
    {
        return createCommandBuffer(FCommandBufferDesc(this, EQueueType::Render));
    }

    ICommandBuffer* IRenderDevice::createComputeCommandBuffer()
    {
        return createCommandBuffer(FCommandBufferDesc(this, EQueueType::Compute));
    }

    ICommandBuffer* IRenderDevice::createCopyCommandBuffer()
    {
        return createCommandBuffer(FCommandBufferDesc(this, EQueueType::Copy));
    }

    IRenderDevice* createRenderDevice(const FRenderDeviceDesc& deviceDesc)
    {
        IRenderDevice* device = nullptr;
        switch (deviceDesc.deviceType)
        {
        case ERenderDeviceType::None:
            return nullptr;
#ifdef LUMA_BUILD_VULKAN
        case ERenderDeviceType::Vulkan:
            device = new Vulkan::FRenderDeviceImpl();
            break;
#endif
        default: return nullptr;
        }

        if (!device->initialize(deviceDesc))
        {
            delete device;
            return nullptr;
        }
        return device;
    }
}
