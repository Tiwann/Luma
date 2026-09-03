#include "Luma/Rendering/GpuDevice.h"
#include "Luma/Asset/Material.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Runtime/FileUtils.h"

#ifdef LUMA_BUILD_WEBGPU
#include "WebGPU/GpuDevice.h"
#endif

#ifdef LUMA_BUILD_VULKAN
#include "Luma/Vulkan/GpuDeviceImpl.h"
#endif

namespace Luma
{
    IShaderProgram* IGpuDevice::createShader(FStringView filepath)
    {
        TArray<uint8_t> fileContent = FileUtils::readToBuffer(filepath);
        return createShader(TBufferView(fileContent.data(), fileContent.size()));
    }

    IShaderProgram* IGpuDevice::createShader(TBufferView<uint8_t> shaderCode)
    {
        return nullptr;
        //return createShader(FShaderDesc(this, shaderCode));
    }

    ICommandBuffer* IGpuDevice::createCommandBuffer(IQueue* queue)
    {
        return createCommandBuffer({this, queue});
    }


    ISampler* IGpuDevice::getOrCreateSampler(const FSamplerDesc& samplerDesc)
    {
        ISampler*& sampler = m_PerDescSamplers[samplerDesc];
        if (sampler) return sampler;
        sampler = createSampler(samplerDesc);
        m_PerDescSamplers[samplerDesc] = sampler;
        return sampler;
    }

    IFence* IGpuDevice::createFence(uint64_t initialValue)
    {
        return createFence(FFenceDesc(this, initialValue));
    }

    FMaterial* IGpuDevice::createMaterial(const FMaterialDesc& materialDesc)
    {
        FMaterialDesc desc(materialDesc);
        desc.device = this;

        FMaterial* material = new FMaterial();
        if (!material->initialize(desc))
        {
            delete material;
            return nullptr;
        }
        return material;
    }

    IGpuDevice* createGpuDevice(const FGpuDeviceDesc& deviceDesc)
    {
        IGpuDevice* device = nullptr;
        switch (deviceDesc.deviceType)
        {
        case EGpuDeviceType::None:
            return nullptr;
        case EGpuDeviceType::Auto:
            {
#ifdef LUMA_PLATFORM_WINDOWS
    #ifdef LUMA_BUILD_D3D12 // D3D12 has priority on Windows
                device = new D3D12::FGpuDeviceImpl();
                break;
    #elifdef LUMA_BUILD_VULKAN
                device = new Vulkan::FGpuDeviceImpl();
                break;
    #elifdef LUMA_BUILD_OPENGL
                device = new OpenGL::FGpuDeviceImpl();
                break;
    #else
                return nullptr;
    #endif
#elifdef LUMA_PLATFORM_LINUX
    #ifdef LUMA_BUILD_VULKAN // Vulkan has priority on Linux
                device = new Vulkan::FGpuDeviceImpl();
                break;
    #elifdef LUMA_BUILD_OPENGL
                device = new OpenGL::FGpuDeviceImpl();
                break;
    #else
                return nullptr;
    #endif
                return nullptr;
#elifdef LUMA_PLATFORM_SWITCH
    #ifdef LUMA_BUILD_DEKO3D // Deko3D is the only supported API for Nintendo Switch
                return new Deko3D::FGpuDeviceImpl();
    #else
                return nullptr;
    #endif
#endif
            }
#ifdef LUMA_BUILD_VULKAN
        case EGpuDeviceType::Vulkan:
            device = new Vulkan::FGpuDeviceImpl();
            break;
#elifdef LUMA_BUILD_D3D12
        case EGpuDeviceType::D3D12:
            device = new D3D12::FGpuDeviceImpl();
            break;
#elifdef LUMA_BUILD_OPENGL
        case EGpuDeviceType::OpenGL:
            device = new OpenGL::FGpuDeviceImpl();
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
