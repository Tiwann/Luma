#include "Luma/Rendering/RenderDevice.h"
#include "Luma/Asset/Material.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Runtime/FileUtils.h"

#ifdef LUMA_BUILD_WEBGPU
#include "WebGPU/RenderDevice.h"
#endif

#ifdef LUMA_BUILD_VULKAN
#include "Luma/Vulkan/RenderDeviceImpl.h"
#endif

namespace Luma
{
    IShaderProgram* IRenderDevice::createShader(FStringView filepath)
    {
        TArray<uint8_t> fileContent = FileUtils::readToBuffer(filepath);
        return createShader(TBufferView(fileContent.data(), fileContent.size()));
    }

    IShaderProgram* IRenderDevice::createShader(TBufferView<uint8_t> shaderCode)
    {
        return nullptr;
        //return createShader(FShaderDesc(this, shaderCode));
    }

    ICommandBuffer* IRenderDevice::createCommandBuffer(IQueue* queue)
    {
        return createCommandBuffer({this, queue});
    }


    ISampler* IRenderDevice::getOrCreateSampler(const FSamplerDesc& samplerDesc)
    {
        ISampler*& sampler = m_PerDescSamplers[samplerDesc];
        if (sampler) return sampler;
        sampler = createSampler(samplerDesc);
        m_PerDescSamplers[samplerDesc] = sampler;
        return sampler;
    }

    IFence* IRenderDevice::createFence(uint64_t initialValue)
    {
        return createFence(FFenceDesc(this, initialValue));
    }

    FMaterial* IRenderDevice::createMaterial(const FMaterialDesc& materialDesc)
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

    IRenderDevice* createRenderDevice(const FRenderDeviceDesc& deviceDesc)
    {
        IRenderDevice* device = nullptr;
        switch (deviceDesc.deviceType)
        {
        case ERenderDeviceType::None:
            return nullptr;
        case ERenderDeviceType::Auto:
            {
#ifdef LUMA_PLATFORM_WINDOWS
    #ifdef LUMA_BUILD_D3D12 // D3D12 has priority on Windows
                device = new D3D12::FRenderDeviceImpl();
                break;
    #elifdef LUMA_BUILD_VULKAN
                device = new Vulkan::FRenderDeviceImpl();
                break;
    #elifdef LUMA_BUILD_OPENGL
                device = new OpenGL::FRenderDeviceImpl();
                break;
    #else
                return nullptr;
    #endif
#elifdef LUMA_PLATFORM_LINUX
    #ifdef LUMA_BUILD_VULKAN // Vulkan has priority on Linux
                device = new Vulkan::FRenderDeviceImpl();
                break;
    #elifdef LUMA_BUILD_OPENGL
                device = new OpenGL::FRenderDeviceImpl();
                break;
    #else
                return nullptr;
    #endif
                return nullptr;
#elifdef LUMA_PLATFORM_SWITCH
    #ifdef LUMA_BUILD_DEKO3D // Deko3D is the only supported API for Nintendo Switch
                return new Deko3D::FRenderDeviceImpl();
    #else
                return nullptr;
    #endif
#endif
            }
#ifdef LUMA_BUILD_VULKAN
        case ERenderDeviceType::Vulkan:
            device = new Vulkan::FRenderDeviceImpl();
            break;
#elifdef LUMA_BUILD_D3D12
        case ERenderDeviceType::D3D12:
            device = new D3D12::FRenderDeviceImpl();
            break;
#elifdef LUMA_BUILD_OPENGL
        case ERenderDeviceType::OpenGL:
            device = new OpenGL::FRenderDeviceImpl();
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
