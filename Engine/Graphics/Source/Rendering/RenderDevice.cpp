#include "Luma/Rendering/RenderDevice.h"

#include "Luma/Asset/Material.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/Semaphore.h"

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
        FCommandBufferDesc desc;
        desc.device = this;
        desc.queueType = EQueueType::Render;
        return createCommandBuffer(desc);
    }

    ICommandBuffer* IRenderDevice::createComputeCommandBuffer()
    {
        return createCommandBuffer(FCommandBufferDesc(this, EQueueType::Compute));
    }

    ICommandBuffer* IRenderDevice::createCopyCommandBuffer()
    {
        return createCommandBuffer(FCommandBufferDesc(this, EQueueType::Copy));
    }

    ISampler* IRenderDevice::getOrCreateSampler(const FSamplerDesc& samplerDesc)
    {
        ISampler*& sampler = m_PerDescSamplers[samplerDesc];
        if (sampler) return sampler;
        sampler = createSampler(samplerDesc);
        return sampler;
    }

    ISemaphore* IRenderDevice::createBinarySemaphore()
    {
        return createSemaphore(FSemaphoreDesc(this, ESemaphoreType::Binary, 0));
    }

    ISemaphore* IRenderDevice::createTimelineSemaphore(const uint64_t initialValue)
    {
        return createSemaphore(FSemaphoreDesc(this, ESemaphoreType::Timeline, initialValue));
    }

    FMaterial* IRenderDevice::createMaterial(const FMaterialDesc& materialDesc)
    {
        FMaterial* material = new FMaterial();
        if (!material->initialize(materialDesc))
        {
            delete material;
            return nullptr;
        }
        return material;
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
