#pragma once
#include "Luma/Rendering/RenderDevice.h"
#include "Luma/Graphics/Export.h"

typedef struct WGPUInstanceImpl* WGPUInstance;
typedef struct WGPUDeviceImpl* WGPUDevice;
typedef struct WGPUAdapterImpl* WGPUAdapter;

namespace luma::wgpu
{
    class LUMA_GRAPHICS_API FRenderDevice : public IRenderDevice
    {
    public:
        ERenderDeviceType getDeviceType() override;
        bool initialize(const FRenderDeviceDesc& deviceDesc) override;
        void destroy() override;

        IBuffer* createBuffer(const FBufferDesc& bufferDesc) override;
        ITexture* createTexture(const FTextureDesc& textureDesc) override;
        IShader* createShader(const FShaderDesc& shaderDesc) override;
        ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& commandBufferDesc) override;
        ISampler* createSampler(const FSamplerDesc& samplerDesc) override;
    private:
        WGPUInstance m_Instance = nullptr;
        WGPUAdapter m_Adapter = nullptr;
        WGPUDevice m_Device = nullptr;
    };
}
