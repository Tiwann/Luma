#include "Luma/Rendering/GPUDevice.h"
#include "Luma/Asset/Material.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Runtime/FileUtils.h"

#ifdef LUMA_BUILD_VULKAN
#include "Luma/Vulkan/GPUDeviceImpl.h"
#endif

#ifdef LUMA_BUILD_D3D12
#include "Luma/D3D12/GPUDeviceImpl.h"
#endif

#ifdef LUMA_BUILD_OPENGL
#include "Luma/OpenGL/GPUDeviceImpl.h"
#endif

#ifdef LUMA_BUILD_WEBGPU
#include "Luma/WebGPU/GPUDeviceImpl.h"
#endif

#ifdef LUMA_BUILD_DEKO3D
#include "Luma/Deko3D/GPUDeviceImpl.h"
#endif


namespace Luma
{
    void IGPUDevice::setVSync(const bool enabled)
    {
        ISwapchain* swapchain = getSwapchain();
        if (!swapchain) return;

        waitIdle();

        FSwapchainDesc swapchainDesc = swapchain->getDesc();
        swapchainDesc.presentMode = enabled ? EPresentMode::Fifo : EPresentMode::Immediate;
        swapchain->initialize(swapchainDesc);
    }

    IBuffer* IGPUDevice::createBuffer(EBufferUsage usage, uint64_t size, bool alwaysMapped)
    {
        FBufferDesc desc;
        desc.device = this;
        desc.size = size;
        desc.usage = usage;
        desc.alwaysMapped = alwaysMapped;

        return createBuffer(desc);
    }

    IShader* IGPUDevice::createShader(FStringView vertexPath, FStringView fragmentPath)
    {
        const TArray<uint8_t> vertexByteCode = FileUtils::readToBuffer(vertexPath);
        const TArray<uint8_t> fragmentByteCode = FileUtils::readToBuffer(fragmentPath);

        if (vertexByteCode.isEmpty() || fragmentByteCode.isEmpty())
            return nullptr;

        TArray<FShaderCode> shaderCodes
        {
            {EShaderStage::Vertex, vertexByteCode},
            {EShaderStage::Fragment, fragmentByteCode}
        };

        FShaderDesc desc;
        desc.device = this;
        desc.shaderCodes = shaderCodes;
        return createShader(desc);
    }

    ICommandBuffer* IGPUDevice::createCommandBuffer(IQueue* queue)
    {
        return createCommandBuffer({this, queue});
    }


    ISampler* IGPUDevice::getOrCreateSampler(const FSamplerDesc& samplerDesc)
    {
        ISampler*& sampler = m_PerDescSamplers[samplerDesc];
        if (sampler) return sampler;
        sampler = createSampler(samplerDesc);
        m_PerDescSamplers[samplerDesc] = sampler;
        return sampler;
    }

    IFence* IGPUDevice::createFence(uint64_t initialValue)
    {
        return createFence(FFenceDesc(this, initialValue));
    }

    FMaterial* IGPUDevice::createMaterial(const FMaterialDesc& materialDesc)
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

    IGPUDevice* createGPUDevice(const FGPUDeviceDesc& deviceDesc)
    {
        IGPUDevice* device = nullptr;
        switch (deviceDesc.deviceType)
        {
        case EGPUDeviceType::None:
            return nullptr;
        case EGPUDeviceType::Auto:
            {
#ifdef LUMA_PLATFORM_WINDOWS
    #ifdef LUMA_BUILD_D3D12 // D3D12 has priority on Windows
                device = new D3D12::FGpuDeviceImpl();
                break;
    #elifdef LUMA_BUILD_VULKAN
                device = new Vulkan::FGPUDeviceImpl();
                break;
    #elifdef LUMA_BUILD_OPENGL
                device = new OpenGL::FGpuDeviceImpl();
                break;
    #elifdef LUMA_BUILD_WEBGPU
                device = new WebGPU::FGpuDeviceImpl();
    #else
                return nullptr;
    #endif
#elifdef LUMA_PLATFORM_LINUX
    #ifdef LUMA_BUILD_VULKAN // Vulkan has priority on Linux
                device = new Vulkan::FGPUDeviceImpl();
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
        case EGPUDeviceType::Vulkan:
            device = new Vulkan::FGPUDeviceImpl();
            break;
#endif
#ifdef LUMA_BUILD_D3D12
        case EGPUDeviceType::D3D12:
            device = new D3D12::FGpuDeviceImpl();
            break;
#endif
#ifdef LUMA_BUILD_OPENGL
        case EGPUDeviceType::OpenGL:
            device = new OpenGL::FGpuDeviceImpl();
            break;
#endif
#ifdef LUMA_BUILD_WEBGPU
        case EGPUDeviceType::WebGPU:
            device = new WebGPU::FGpuDeviceImpl();
            break;
#endif
#ifdef LUMA_BUILD_DEKO3D
        case EGPUDeviceType::Deko3D:
            device = new Deko3D::FGpuDeviceImpl();
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

    IGPUDevice* createGPUDevice(IWindow* window, EGPUDeviceType deviceType, ESwapchainBuffering buffering, bool vsync)
    {
        return createGPUDevice({window, deviceType, buffering, vsync});
    }
}
