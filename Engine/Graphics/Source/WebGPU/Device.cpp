#include "RenderDevice.h"
#include <webgpu/webgpu.h>

namespace Luma::WebGPU
{
    ERenderDeviceType FRenderDevice::getDeviceType()
    {
        return ERenderDeviceType::WebGPU;
    }

    bool FRenderDevice::initialize(const FRenderDeviceDesc& deviceDesc)
    {
        constexpr WGPUInstanceDescriptor descriptor = WGPU_INSTANCE_DESCRIPTOR_INIT;
        m_Instance = wgpuCreateInstance(&descriptor);

        WGPURequestAdapterOptions adapterOpts = {};
        adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;

        const auto onAdapterRequestEnded = [](
            WGPURequestAdapterStatus status,
            WGPUAdapter adapter,
            WGPUStringView message,
            void* userdata1,
            void* userdata2)
        {
            (void)status;
            (void)message;
            (void)userdata2;

            if (status != WGPURequestAdapterStatus_Success)
                return;

            if (WGPUAdapter* selectedAdapter = static_cast<WGPUAdapter*>(userdata1))
                *selectedAdapter = adapter;
        };

        WGPUAdapter selectedAdapter = nullptr;
        WGPURequestAdapterCallbackInfo adapterCallbackInfo = WGPU_REQUEST_ADAPTER_CALLBACK_INFO_INIT;
        adapterCallbackInfo.userdata1 = &selectedAdapter;
        adapterCallbackInfo.callback = onAdapterRequestEnded;
        wgpuInstanceRequestAdapter(m_Instance, &adapterOpts, adapterCallbackInfo);
        wgpuInstanceProcessEvents(m_Instance);
        if (!selectedAdapter) return false;
        m_Adapter = selectedAdapter;

        WGPUDeviceDescriptor desc = WGPU_DEVICE_DESCRIPTOR_INIT;
        desc.deviceLostCallbackInfo.callback = [](
            WGPUDevice const* device,
            WGPUDeviceLostReason reason,
            WGPUStringView message,
            void*, void*)
            {
                // Log or assert here — device loss is unrecoverable
            };


        WGPUDevice selectedDevice = nullptr;
        WGPURequestDeviceCallbackInfo deviceCallbackInfo = WGPU_REQUEST_DEVICE_CALLBACK_INFO_INIT;
        deviceCallbackInfo.userdata1 = &selectedDevice;
        deviceCallbackInfo.callback = onDeviceRequestEnded;
        wgpuAdapterRequestDevice(m_Adapter, &desc, deviceCallbackInfo);
        wgpuInstanceProcessEvents(m_Instance);
        if (!selectedDevice) return false;
        m_Device = selectedDevice;
        return true;
    }

    void FRenderDevice::destroy()
    {
        wgpuAdapterRelease(m_Adapter);
        wgpuInstanceRelease(m_Instance);
    }

    IBuffer* FRenderDevice::createBuffer(const FBufferDesc& bufferDesc)
    {
        return nullptr;
    }

    ITexture* FRenderDevice::createTexture(const FTextureDesc& textureDesc)
    {
        return nullptr;
    }

    IShader* FRenderDevice::createShader(const FShaderDesc& shaderDesc)
    {
        return nullptr;
    }

    ICommandBuffer* FRenderDevice::createCommandBuffer(const FCommandBufferDesc& commandBufferDesc)
    {
        return nullptr;
    }

    ISampler* FRenderDevice::createSampler(const FSamplerDesc& samplerDesc)
    {
        return nullptr;
    }
}
