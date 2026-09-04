#include "Luma/WebGPU/GpuDeviceImpl.h"
#include <webgpu/webgpu.h>
#include <glfw3webgpu.h>

#include "Luma/Runtime/DesktopWindow.h"

#define PRINT_STR(x) do { std::cout << FStringView(x.data, x.length) << std::endl; } while(0)

namespace Luma::WebGPU
{
    bool FGpuDeviceImpl::initialize(const FGpuDeviceDesc& deviceDesc)
    {
        constexpr WGPUInstanceFeatureName requiredFeatures[] = { WGPUInstanceFeatureName_ShaderSourceSPIRV };

        WGPUInstanceDescriptor descriptor{};
        descriptor.requiredFeatures = requiredFeatures;
        descriptor.requiredFeatureCount = std::size(requiredFeatures);
        descriptor.requiredLimits = nullptr;

        m_Instance = wgpuCreateInstance(&descriptor);
        if (!m_Instance) return false;

        WGPURequestAdapterOptions options{};
        options.powerPreference = WGPUPowerPreference_HighPerformance;
        options.featureLevel = WGPUFeatureLevel_Core;

        WGPURequestAdapterCallbackInfo callbackInfo{};
        callbackInfo.userdata1 = this;
        callbackInfo.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void* userdata1, void* userdata2)
        {
            if (status == WGPURequestAdapterStatus_Error)
            {
                PRINT_STR(message);
                return;
            }

            FGpuDeviceImpl* thisDevice = static_cast<FGpuDeviceImpl*>(userdata1);
            thisDevice->m_Adapter = adapter;
        };
        wgpuInstanceRequestAdapter(m_Instance, &options, callbackInfo);

        constexpr WGPUFeatureName deviceFeatures[]{
            WGPUFeatureName_CoreFeaturesAndLimits,
            WGPUFeatureName_Depth32FloatStencil8,
            WGPUFeatureName_TextureComponentSwizzle
        };

        WGPULimits limits = WGPU_LIMITS_INIT;

        WGPUDeviceDescriptor deviceDescriptor{};
        deviceDescriptor.requiredFeatures = deviceFeatures;
        deviceDescriptor.requiredFeatureCount = std::size(deviceFeatures);
        deviceDescriptor.deviceLostCallbackInfo = {
            .callback = [](WGPUDevice const* device, WGPUDeviceLostReason reason, WGPUStringView message, void* userdata1, void* userdata2)
            {
                std::cout << "[WEBGPU] Device lost!\n";
                PRINT_STR(message);
                std::exit(EXIT_FAILURE);
            },
            .userdata1 = this
        };
        deviceDescriptor.uncapturedErrorCallbackInfo = {
            .callback = [](WGPUDevice const* device, WGPUErrorType type, WGPUStringView message, void* userdata1, void* userdata2)
            {
                PRINT_STR(message);
                std::exit(EXIT_FAILURE);
            },
            .userdata1 = this
        };
        deviceDescriptor.requiredLimits = &limits;

        WGPURequestDeviceCallbackInfo requestDeviceCallbackInfo{};
        requestDeviceCallbackInfo.userdata1 = this;
        requestDeviceCallbackInfo.callback = [](WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, void* userdata1, void* userdata2)
        {
            if (status == WGPURequestDeviceStatus_Error)
            {
                PRINT_STR(message);
                return;
            }

            FGpuDeviceImpl* thisDevice = static_cast<FGpuDeviceImpl*>(userdata1);
            thisDevice->m_Handle = device;
        };
        wgpuAdapterRequestDevice(m_Adapter, &deviceDescriptor, requestDeviceCallbackInfo);


        m_DefaultQueue.initialize();

        GLFWwindow* window = static_cast<FDesktopWindow*>(deviceDesc.window)->getHandle();
        m_Surface = glfwCreateWindowWGPUSurface(m_Instance, window);
        if (!m_Surface) return false;

        
        return true;
    }

    void FGpuDeviceImpl::destroy()
    {
    }

    bool FGpuDeviceImpl::beginFrame()
    {
        return false;
    }

    void FGpuDeviceImpl::endFrame()
    {
    }

    void FGpuDeviceImpl::present()
    {
    }

    void FGpuDeviceImpl::waitIdle()
    {
    }

    uint32_t FGpuDeviceImpl::getTextureCount() const
    {
        return 0;
    }

    uint32_t FGpuDeviceImpl::getFrameIndex() const
    {
        return 0;
    }

    bool FGpuDeviceImpl::hasVSync()
    {
        return false;
    }

    ISwapchain* FGpuDeviceImpl::getSwapchain()
    {
        return nullptr;
    }

    IBuffer* FGpuDeviceImpl::createBuffer(const FBufferDesc& bufferDesc)
    {
        return nullptr;
    }

    ITexture* FGpuDeviceImpl::createTexture(const FTextureDesc& textureDesc)
    {
        return nullptr;
    }

    ITextureView* FGpuDeviceImpl::createTextureView(const FTextureViewDesc& textureViewDesc)
    {
        return nullptr;
    }

    IShaderProgram* FGpuDeviceImpl::createShader(const FShaderDesc& shaderDesc)
    {
        return nullptr;
    }

    ICommandBuffer* FGpuDeviceImpl::createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc)
    {
        return nullptr;
    }

    ICommandBuffer* FGpuDeviceImpl::getCommandBuffer()
    {
        return nullptr;
    }

    ISampler* FGpuDeviceImpl::createSampler(const FSamplerDesc& samplerDesc)
    {
        return nullptr;
    }

    IRenderPipeline* FGpuDeviceImpl::createRenderPipeline(const FRenderPipelineDesc& pipelineDesc)
    {
        return nullptr;
    }

    IComputePipeline* FGpuDeviceImpl::createComputePipeline(const FComputePipelineDesc& pipelineDesc)
    {
        return nullptr;
    }

    IFence* FGpuDeviceImpl::createFence(const FFenceDesc& fenceDesc)
    {
        return nullptr;
    }

    ITextureView* FGpuDeviceImpl::getAcquiredSwapchainTextureView()
    {
        return nullptr;
    }

    void FGpuDeviceImpl::writeSamplerDescriptor(IBuffer* buffer, uint64_t offset, const ISampler* sampler)
    {
        IGpuDevice::writeSamplerDescriptor(buffer, offset, sampler);
    }

    void FGpuDeviceImpl::writeTextureDescriptor(IBuffer* buffer, uint64_t offset, const ITexture* texture,
        ETextureBindingType bindingType)
    {
        IGpuDevice::writeTextureDescriptor(buffer, offset, texture, bindingType);
    }

    void FGpuDeviceImpl::writeBufferDescriptor(IBuffer* buffer, uint64_t offset, const IBuffer* bufferResource,
        uint64_t resourceOffset, uint64_t resourceSize, EBufferBindingType bindingType)
    {
        IGpuDevice::writeBufferDescriptor(buffer, offset, bufferResource, resourceOffset, resourceSize, bindingType);
    }
}
