#include "Luma/WebGPU/GPUDeviceImpl.h"
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <webgpu/webgpu.h>
#include <glfw3webgpu.h>

#include "Luma/Runtime/DesktopWindow.h"

#define PRINT_STR(x) do { std::cout << FStringView(x.data, x.length) << std::endl; } while(0)

namespace Luma::WebGPU
{
    bool FGPUDeviceImpl::initialize(const FGPUDeviceDesc& deviceDesc)
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

            FGPUDeviceImpl* thisDevice = static_cast<FGPUDeviceImpl*>(userdata1);
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

            FGPUDeviceImpl* thisDevice = static_cast<FGPUDeviceImpl*>(userdata1);
            thisDevice->m_Handle = device;
        };
        wgpuAdapterRequestDevice(m_Adapter, &deviceDescriptor, requestDeviceCallbackInfo);


        m_DefaultQueue.initialize();

        GLFWwindow* window = static_cast<FDesktopWindow*>(deviceDesc.window)->getHandle();
        HWND hwnd = glfwGetWin32Window(window);
        HINSTANCE hinstance = GetModuleHandle(NULL);

        WGPUSurfaceSourceWindowsHWND fromWindowsHWND;
        fromWindowsHWND.chain.sType = WGPUSType_SurfaceSourceWindowsHWND;
        fromWindowsHWND.chain.next = NULL;
        fromWindowsHWND.hinstance = hinstance;
        fromWindowsHWND.hwnd = hwnd;

        WGPUSurfaceDescriptor surfaceDescriptor;
        surfaceDescriptor.nextInChain = &fromWindowsHWND.chain;
        surfaceDescriptor.label = (WGPUStringView){ NULL, WGPU_STRLEN };

        m_Surface = wgpuInstanceCreateSurface(m_Instance, &surfaceDescriptor);

        m_Window = deviceDesc.window;
        m_Window->resizedEvent.bind([this](uint32_t, uint32_t) { m_Swapchain.invalidate(); });

        FSwapchainDesc swapchainDesc;
        swapchainDesc.device = this;
        swapchainDesc.buffering = deviceDesc.buffering;
        swapchainDesc.format = EFormat::R8G8B8A8_SRGB;
        swapchainDesc.width = deviceDesc.window->getWidth();
        swapchainDesc.height = deviceDesc.window->getHeight();
        swapchainDesc.presentMode = deviceDesc.vSync ? EPresentMode::Fifo : EPresentMode::Immediate;
        if (!m_Swapchain.initialize(swapchainDesc))
        {
            std::wcerr << L"Failed to initialize swapchain!\n";
            return false;
        }

        return true;
    }

    void FGPUDeviceImpl::destroy()
    {
        m_Swapchain.destroy();

        if (m_Surface)
        {
            wgpuSurfaceRelease(m_Surface);
            m_Surface = nullptr;
        }

        if (m_Handle)
        {
            wgpuDeviceRelease(m_Handle);
            m_Handle = nullptr;
        }

        if (m_Adapter)
        {
            wgpuAdapterRelease(m_Adapter);
            m_Adapter = nullptr;
        }

        if (m_Instance)
        {
            wgpuInstanceRelease(m_Instance);
            m_Instance = nullptr;
        }
    }

    bool FGPUDeviceImpl::beginFrame()
    {
        if (!m_Window) return false;
        if (!m_Window->isAvailable()) return false;

        if (!m_Swapchain.isValid())
        {
            waitIdle();

            m_Swapchain.resize(m_Window->getWidth(), m_Window->getHeight());
            m_FrameIndex = 0;
            return false;
        }

        if (!m_Swapchain.acquireNextTexture(m_SwapchainImageIndex))
        {
            m_Swapchain.invalidate();
            return false;
        }

        return true;
    }

    void FGPUDeviceImpl::endFrame()
    {
    }

    void FGPUDeviceImpl::present()
    {
        wgpuSurfacePresent(m_Swapchain.getHandle());
        m_FrameIndex = (m_FrameIndex + 1) % NUM_FRAMES_IN_FLIGHT;
    }

    void FGPUDeviceImpl::waitIdle()
    {
    }

    uint32_t FGPUDeviceImpl::getTextureCount() const
    {
        return m_Swapchain.getTextureCount();
    }

    uint32_t FGPUDeviceImpl::getFrameIndex() const
    {
        return m_FrameIndex;
    }

    bool FGPUDeviceImpl::hasVSync()
    {
        return m_Swapchain.hasVSync();
    }

    ISwapchain* FGPUDeviceImpl::getSwapchain()
    {
        return &m_Swapchain;
    }

    IBuffer* FGPUDeviceImpl::createBuffer(const FBufferDesc& bufferDesc)
    {
        return nullptr;
    }

    ITexture* FGPUDeviceImpl::createTexture(const FTextureDesc& textureDesc)
    {
        return nullptr;
    }

    ITextureView* FGPUDeviceImpl::createTextureView(const FTextureViewDesc& textureViewDesc)
    {
        return nullptr;
    }

    IShader* FGPUDeviceImpl::createShader(const FShaderDesc& shaderDesc)
    {
        return nullptr;
    }

    ICommandBuffer* FGPUDeviceImpl::createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc)
    {
        return nullptr;
    }

    ICommandBuffer* FGPUDeviceImpl::getCommandBuffer()
    {
        return nullptr;
    }

    ISampler* FGPUDeviceImpl::createSampler(const FSamplerDesc& samplerDesc)
    {
        return nullptr;
    }

    IRenderPipeline* FGPUDeviceImpl::createRenderPipeline(const FRenderPipelineDesc& pipelineDesc)
    {
        return nullptr;
    }

    IComputePipeline* FGPUDeviceImpl::createComputePipeline(const FComputePipelineDesc& pipelineDesc)
    {
        return nullptr;
    }

    IFence* FGPUDeviceImpl::createFence(const FFenceDesc& fenceDesc)
    {
        return nullptr;
    }

    ITextureView* FGPUDeviceImpl::getAcquiredSwapchainTextureView()
    {
        return m_Swapchain.getTextureView(m_SwapchainImageIndex);
    }

    void FGPUDeviceImpl::writeSamplerDescriptor(IBuffer* buffer, uint64_t offset, const ISampler* sampler)
    {
        IGPUDevice::writeSamplerDescriptor(buffer, offset, sampler);
    }

    void FGPUDeviceImpl::writeTextureDescriptor(IBuffer* buffer, uint64_t offset, const ITexture* texture,
        ETextureBindingType bindingType)
    {
        IGPUDevice::writeTextureDescriptor(buffer, offset, texture, bindingType);
    }

    void FGPUDeviceImpl::writeBufferDescriptor(IBuffer* buffer, uint64_t offset, const IBuffer* bufferResource,
        uint64_t resourceOffset, uint64_t resourceSize, EBufferBindingType bindingType)
    {
        IGPUDevice::writeBufferDescriptor(buffer, offset, bufferResource, resourceOffset, resourceSize, bindingType);
    }
}
