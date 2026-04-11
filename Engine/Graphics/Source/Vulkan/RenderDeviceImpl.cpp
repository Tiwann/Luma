#include "RenderDeviceImpl.h"
#include <Volk/volk.h>
#include <iostream>

namespace luma::vulkan
{
    ERenderDeviceType FRenderDeviceImpl::getDeviceType()
    {
        return ERenderDeviceType::Vulkan;
    }

    bool FRenderDeviceImpl::initialize(const FRenderDeviceDesc& deviceDesc)
    {
        if (!deviceDesc.window)
        {
            std::wcout << L"Failed to initialize render device: Invalid window!\n";
            return false;
        }

        if (VK_FAILED(volkInitialize()))
        {
            std::wcout << L"Failed to initialize Volk!\n";
            return false;
        }

        return false;
    }

    void FRenderDeviceImpl::destroy()
    {

    }

    void FRenderDeviceImpl::beginFrame()
    {
    }

    void FRenderDeviceImpl::endFrame()
    {
    }

    void FRenderDeviceImpl::present()
    {
    }

    void FRenderDeviceImpl::waitIdle()
    {
    }

    uint32_t FRenderDeviceImpl::getFrameCount()
    {
        return 0;
    }

    uint32_t FRenderDeviceImpl::getCurrentFrameIndex()
    {
        return 0;
    }

    bool FRenderDeviceImpl::hasVSync()
    {
        return IRenderDevice::hasVSync();
    }

    IBuffer* FRenderDeviceImpl::createBuffer(const FBufferDesc& bufferDesc)
    {
        return nullptr;
    }

    ITexture* FRenderDeviceImpl::createTexture(const FTextureDesc& textureDesc)
    {
        return nullptr;
    }

    IShader* FRenderDeviceImpl::createShader(const FShaderDesc& shaderDesc)
    {
        return nullptr;
    }

    ICommandBuffer* FRenderDeviceImpl::createCommandBuffer(const FCommandBufferDesc& commandBufferDesc)
    {
        return nullptr;
    }

    ISampler* FRenderDeviceImpl::createSampler(const FSamplerDesc& samplerDesc)
    {
        return nullptr;
    }

    IRenderCommandBuffer* FRenderDeviceImpl::createRenderCommandBuffer(const FCommandBufferDesc& cmdBufferDesc)
    {
        return nullptr;
    }

    IComputeCommandBuffer* FRenderDeviceImpl::createComputeCommandBuffer(const FCommandBufferDesc& cmdBufferDesc)
    {
        return nullptr;
    }

    ICopyCommandBuffer* FRenderDeviceImpl::createCopyCommandBuffer(const FCommandBufferDesc& cmdBufferDesc)
    {
        return nullptr;
    }

    IGraphicsPipeline* FRenderDeviceImpl::createGraphicsPipeline(const FGraphicsPipelineDesc& pipelineDesc)
    {
        return nullptr;
    }

    IComputePipeline* FRenderDeviceImpl::createComputePipeline(const FComputePipelineDesc& pipelineDesc)
    {
        return nullptr;
    }

    IFence* FRenderDeviceImpl::createFence(const FFenceDesc& fenceDesc)
    {
        return nullptr;
    }
}
