#pragma once
#include "Luma/Graphics/Export.h"
#include "SwpchainBuffering.h"
#include "RenderDeviceType.h"
#include "Luma/Memory/RefCounted.h"
#include <cstdint>


namespace Luma
{
    struct FBufferDesc;
    struct FTextureDesc;
    struct FTextureViewDesc;
    struct FShaderDesc;
    struct FCommandBufferDesc;
    struct FFenceDesc;
    struct FSamplerDesc;
    struct FGraphicsPipelineDesc;
    struct FComputePipelineDesc;


    struct IWindow;
    struct IBuffer;
    struct ITexture;
    struct ITextureView;
    struct IShader;
    struct ICommandBuffer;
    struct IRenderCommandBuffer;
    struct IComputeCommandBuffer;
    struct ICopyCommandBuffer;
    struct IFence;
    struct ISampler;
    struct IGraphicsPipeline;
    struct IComputePipeline;
    struct ISwapchain;
    struct IQueue;


    struct FRenderDeviceDesc
    {
        IWindow* window = nullptr;
        ERenderDeviceType deviceType = ERenderDeviceType::None;
        ESwapchainBuffering buffering = ESwapchainBuffering::None;
        bool vSync = false;
        char padding[3];
    };

    struct LUMA_GRAPHICS_API IRenderDevice : IRefCounted<IRenderDevice>
    {
        ~IRenderDevice() override = default;
        virtual ERenderDeviceType getDeviceType() = 0;
        virtual bool initialize(const FRenderDeviceDesc& deviceDesc) = 0;
        virtual void destroy() = 0;

        virtual bool beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual void present() = 0;
        virtual void waitIdle() = 0;
        virtual uint32_t getFrameCount() = 0;
        virtual uint32_t getCurrentFrameIndex() = 0;
        virtual bool hasVSync() { return false; }

        virtual ISwapchain* getSwapchain() { return nullptr; }
        virtual IQueue* getRenderQueue() { return nullptr; }
        virtual IQueue* getComputeQueue() { return nullptr; }
        virtual IQueue* getCopyQueue() { return nullptr; }

        virtual IBuffer* createBuffer(const FBufferDesc& bufferDesc) const = 0;
        virtual ITexture* createTexture(const FTextureDesc& textureDesc) const = 0;
        virtual ITextureView* createTextureView(const FTextureViewDesc& textureViewDesc) const = 0;
        virtual IShader* createShader(const FShaderDesc& shaderDesc) const = 0;
        virtual ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) const = 0;
        ICommandBuffer* createRenderCommandBuffer();
        ICommandBuffer* createComputeCommandBuffer();
        ICommandBuffer* createCopyCommandBuffer();
        virtual ISampler* createSampler(const FSamplerDesc& samplerDesc) const = 0;
        virtual ISampler* getOrCreateSampler(const FSamplerDesc& samplerDesc) = 0;
        virtual IGraphicsPipeline* createGraphicsPipeline(const FGraphicsPipelineDesc& pipelineDesc) const = 0;
        virtual IComputePipeline* createComputePipeline(const FComputePipelineDesc& pipelineDesc) const = 0;
        virtual IFence* createFence(const FFenceDesc& fenceDesc) const = 0;
    };

    LUMA_GRAPHICS_API IRenderDevice* createRenderDevice(const FRenderDeviceDesc& deviceDesc);
}
