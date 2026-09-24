#include "Luma/Rendering/Device.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/Shader.h"
#include "Luma/Runtime/FileUtils.h"

#ifdef LUMA_BUILD_VULKAN
#include "Luma/Vulkan/Device.h"
#endif

#ifdef LUMA_BUILD_D3D12
#include "Luma/D3D12/Device.h"
#endif

#ifdef LUMA_BUILD_OPENGL
#include "Luma/OpenGL/Device.h"
#endif

#ifdef LUMA_BUILD_WEBGPU
#include "Luma/WebGPU/Device.h"
#endif

#ifdef LUMA_BUILD_DEKO3D
#include "Luma/Deko3D/Device.h"
#endif


namespace Luma::RHI
{
    void Device::setVSync(const bool enabled)
    {
        Swapchain* swapchain = getSwapchain();
        if (!swapchain) return;

        waitIdle();

        SwapchainDesc swapchainDesc = swapchain->getDesc();
        swapchainDesc.presentMode = enabled ? PresentMode::Fifo : PresentMode::Immediate;
        swapchain->initialize(swapchainDesc);
    }

    Buffer* Device::createBuffer(BufferUsage usage, uint64_t size, bool alwaysMapped)
    {
        BufferDesc desc;
        desc.device = this;
        desc.size = size;
        desc.usage = usage;
        desc.alwaysMapped = alwaysMapped;

        return createBuffer(desc);
    }

    Shader* Device::createShader(StringView vertexPath, StringView fragmentPath)
    {
        const Array<uint8_t> vertexByteCode = FileUtils::readToBuffer(vertexPath);
        const Array<uint8_t> fragmentByteCode = FileUtils::readToBuffer(fragmentPath);

        if (vertexByteCode.isEmpty() || fragmentByteCode.isEmpty())
            return nullptr;

        Array<ShaderCode> shaderCodes
        {
            {ShaderStage::Vertex, vertexByteCode},
            {ShaderStage::Fragment, fragmentByteCode}
        };

        ShaderDesc desc;
        desc.device = this;
        desc.shaderCodes = shaderCodes;
        return createShader(desc);
    }

    CommandBuffer* Device::createCommandBuffer(Queue* queue)
    {
        return createCommandBuffer({this, queue});
    }


    Sampler* Device::getOrCreateSampler(const SamplerDesc& samplerDesc)
    {
        Sampler*& sampler = m_PerDescSamplers[samplerDesc];
        if (sampler) return sampler;
        sampler = createSampler(samplerDesc);
        m_PerDescSamplers[samplerDesc] = sampler;
        return sampler;
    }

    Fence* Device::createFence(uint64_t initialValue)
    {
        return createFence(FenceDesc(this, initialValue));
    }

    Device* createDevice(const DeviceDesc& deviceDesc)
    {
        Device* device = nullptr;
        switch (deviceDesc.deviceType)
        {
        case DeviceType::None:
            return nullptr;
        case DeviceType::Auto:
            {
#ifdef LUMA_PLATFORM_WINDOWS
    #ifdef LUMA_BUILD_D3D12 // D3D12 has priority on Windows
                device = new D3D12::FGpuDeviceImpl();
                break;
    #elifdef LUMA_BUILD_VULKAN
                device = new Vulkan::Device();
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
        case DeviceType::Vulkan:
            device = new Vulkan::Device();
            break;
#endif
#ifdef LUMA_BUILD_D3D12
        case DeviceType::D3D12:
            device = new D3D12::FGpuDeviceImpl();
            break;
#endif
#ifdef LUMA_BUILD_OPENGL
        case DeviceType::OpenGL:
            device = new OpenGL::FGpuDeviceImpl();
            break;
#endif
#ifdef LUMA_BUILD_WEBGPU
        case DeviceType::WebGPU:
            device = new WebGPU::FGpuDeviceImpl();
            break;
#endif
#ifdef LUMA_BUILD_DEKO3D
        case DeviceType::Deko3D:
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

    Device* createDevice(Window* window, DeviceType deviceType, SwapchainBuffering buffering, bool vsync)
    {
        return createDevice({window, deviceType, buffering, vsync});
    }

    bool Device::hasVSync()
    {
        return getSwapchain()->hasVSync();
    }
}
