#pragma once
#include "Luma/Rendering/GpuDevice.h"
#include "DekoFwd.h"
#include "QueueImpl.h"
#include "SwapchainImpl.h"
#include "CommandBufferImpl.h"

namespace Luma
{
    class FSwitchWindow;
}

namespace Luma::Deko3d
{
    class LUMA_GRAPHICS_API FGpuDeviceImpl : public IGpuDevice
    {
    public:
        EGpuDeviceType getDeviceType() override;
        bool initialize(const FGpuDeviceDesc& deviceDesc) override;
        void destroy() override;
        bool beginFrame() override;
        void endFrame() override;
        void present() override;
        void waitIdle() override;
        uint32_t getFrameCount() override;
        uint32_t getCurrentFrameIndex() override;
        IBuffer* createBuffer(const FBufferDesc& bufferDesc) override;
        ITexture* createTexture(const FTextureDesc& textureDesc) override;
        ITextureView* createTextureView(const FTextureViewDesc& textureViewDesc) override;
        IShader* createShader(const FShaderDesc& shaderDesc) override;
        ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) override;
        ICommandBuffer* getCommandBuffer() override;
        ISampler* createSampler(const FSamplerDesc& samplerDesc) override;
        IGraphicsPipeline* createGraphicsPipeline(const FGraphicsPipelineDesc& pipelineDesc) override;
        IComputePipeline* createComputePipeline(const FComputePipelineDesc& pipelineDesc) override;
        IFence* createFence(const FFenceDesc& fenceDesc) override;
        ISemaphore* createSemaphore(const FSemaphoreDesc& semaphoreDesc) override;
        ITextureView* getAcquiredSwapchainTextureView() override;

        DkDevice getHandle() const;
        FSwitchWindow* getWindow() const;
        IQueue* getRenderQueue() override;
        IQueue* getComputeQueue() override;
        IQueue* getCopyQueue() override;
    private:
        DkDevice m_Device = nullptr;
        FSwitchWindow* m_Window = nullptr;
        FSwapchainImpl m_Swapchain;
        FQueueImpl m_RenderQueue;
        FQueueImpl m_ComputeQueue;
        FQueueImpl m_CopyQueue;
        FCommandBufferImpl m_CmdBuffers[3];

        uint32_t m_CurrentFrameIndex = 0;
        uint32_t m_SwapchainImageIndex = 0;
    };
}
