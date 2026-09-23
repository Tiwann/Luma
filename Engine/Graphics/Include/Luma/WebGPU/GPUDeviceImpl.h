#pragma once
#include "QueueImpl.h"
#include "SwapchainImpl.h"
#include "Luma/Rendering/Device.h"
#include "Luma/Rendering/Constants.h"
#include "WgpuFwd.h"

namespace Luma::WebGPU
{
    class FGPUDeviceImpl : public IGPUDevice
    {
    public:
        EGpuDeviceType getDeviceType() override { return EGpuDeviceType::WebGPU; }

        bool initialize(const FGPUDeviceDesc& deviceDesc) override;
        void destroy() override;
        bool beginFrame() override;
        void endFrame() override;
        void present() override;
        void waitIdle() override;
        uint32_t getTextureCount() const override;
        uint32_t getFrameIndex() const override;
        bool hasVSync() override;
        ISwapchain* getSwapchain() override;
        IQueue* getRenderQueue() override { return &m_DefaultQueue; }
        IQueue* getComputeQueue() override { return &m_DefaultQueue; }
        IQueue* getCopyQueue() override { return &m_DefaultQueue; }
        IBuffer* createBuffer(const FBufferDesc& bufferDesc) override;
        ITexture* createTexture(const FTextureDesc& textureDesc) override;
        ITextureView* createTextureView(const FTextureViewDesc& textureViewDesc) override;
        IShader* createShader(const FShaderDesc& shaderDesc) override;
        ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) override;
        ICommandBuffer* getCommandBuffer() override;
        ISampler* createSampler(const FSamplerDesc& samplerDesc) override;
        IRenderPipeline* createRenderPipeline(const FRenderPipelineDesc& pipelineDesc) override;
        IComputePipeline* createComputePipeline(const FComputePipelineDesc& pipelineDesc) override;
        IFence* createFence(const FFenceDesc& fenceDesc) override;
        ITextureView* getAcquiredSwapchainTextureView() override;
        void writeSamplerDescriptor(IBuffer* buffer, uint64_t offset, const ISampler* sampler) override;
        void writeTextureDescriptor(IBuffer* buffer, uint64_t offset, const ITexture* texture,
            ETextureBindingType bindingType) override;
        void writeBufferDescriptor(IBuffer* buffer, uint64_t offset, const IBuffer* bufferResource,
            uint64_t resourceOffset, uint64_t resourceSize, EBufferBindingType bindingType) override;

        WGPUInstance getInstance() const { return m_Instance; }
        WGPUAdapter getAdapter() const { return m_Adapter; }
        WGPUDevice getHandle() const { return m_Handle; }
        WGPUSurface getSurface() const { return m_Surface; }
    private:
        WGPUInstance m_Instance = nullptr;
        WGPUAdapter m_Adapter = nullptr;
        WGPUDevice m_Handle = nullptr;
        WGPUSurface m_Surface = nullptr;
        IWindow* m_Window = nullptr;

        FQueueImpl m_DefaultQueue{this};
        FSwapchainImpl m_Swapchain;
        uint32_t m_SwapchainImageIndex = 0;
        uint32_t m_FrameIndex = 0;
    };
}