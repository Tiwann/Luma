#pragma once
#include "Luma/Rendering/RenderDevice.h"
#include "Luma/Rendering/QueueType.h"
#include "D3D12Fwd.h"
#include "QueueImpl.h"
#include "SwapchainImpl.h"

#define DX_FAILED(result) (result) < 0

namespace Luma
{
    class FDesktopWindow;
}

namespace Luma::D3D12
{
    class FRenderDeviceImpl : public IRenderDevice
    {
    public:
        ERenderDeviceType getDeviceType() override;
        bool initialize(const FRenderDeviceDesc& deviceDesc) override;
        void destroy() override;
        bool beginFrame() override;
        void endFrame() override;
        void present() override;
        void waitIdle() override;
        uint32_t getFrameCount() const override;
        uint32_t getCurrentFrameIndex() override;
        bool hasVSync() override;
        ISwapchain* getSwapchain() override;
        IQueue* getRenderQueue() override;
        IQueue* getComputeQueue() override;
        IQueue* getCopyQueue() override;
        IBuffer* createBuffer(const FBufferDesc& bufferDesc) override;
        ITexture* createTexture(const FTextureDesc& textureDesc) override;
        ITextureView* createTextureView(const FTextureViewDesc& textureViewDesc) override;
        IShaderProgram* createShader(const FShaderDesc& shaderDesc) override;
        ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) override;
        ICommandBuffer* getCommandBuffer() override;
        ISampler* createSampler(const FSamplerDesc& samplerDesc) override;
        IRenderPipeline* createRenderPipeline(const FRenderPipelineDesc& pipelineDesc) override;
        IComputePipeline* createComputePipeline(const FComputePipelineDesc& pipelineDesc) override;
        IFence* createFence(const FFenceDesc& fenceDesc) override;
        ISemaphore* createSemaphore(const FSemaphoreDesc& semaphoreDesc) override;
        ITextureView* getAcquiredSwapchainTextureView() override;
        void writeSamplerDescriptor(IBuffer* buffer, uint64_t offset, const ISampler* sampler) override;
        void writeTextureDescriptor(IBuffer* buffer, uint64_t offset, const ITexture* texture,ETextureBindingType bindingType) override;
        void writeBufferDescriptor(IBuffer* buffer, uint64_t offset, const IBuffer* bufferResource,uint64_t resourceOffset, uint64_t resourceSize, EBufferBindingType bindingType) override;

        ID3D12Device15* getHandle() const { return m_Handle; }
        ID3D12Allocator* getAllocator() const { return m_Allocator; }
        FDesktopWindow* getWindow() const { return m_Window; }
        IDXGIFactory7* getFactory() const { return m_Factory; }
        IDXGIAdapter4* getAdapter() const { return m_Adapter; }

        ID3D12CommandSignature* getDrawIndirectSignature() const { return m_DrawIndirectSignature; };
        ID3D12CommandSignature* getDrawIndexedIndirectSignature() const { return m_DrawIndexedIndirectSignature; };
        ID3D12CommandSignature* getDispatchIndirectSignature() const { return m_DispatchIndirectSignature; };
        ID3D12CommandAllocator* getCommandAllocator(EQueueType queueType);
    private:
        bool createCommandSignatures();

    private:
        FDesktopWindow* m_Window = nullptr;
        ID3D12Device15* m_Handle = nullptr;
        IDXGIFactory7* m_Factory = nullptr;
        IDXGIAdapter4* m_Adapter = nullptr;
        ID3D12InfoQueue1* m_InfoQueue = nullptr;
#if defined(LUMA_DEBUG) || defined(LUMA_DEV)
        ID3D12Debug6* m_Debug = nullptr;
#endif

        ID3D12CommandSignature* m_DrawIndirectSignature = nullptr;
        ID3D12CommandSignature* m_DrawIndexedIndirectSignature = nullptr;
        ID3D12CommandSignature* m_DispatchIndirectSignature = nullptr;

        ID3D12Allocator* m_Allocator = nullptr;
        ID3D12CommandAllocator* m_RenderCmdAllocator = nullptr;
        ID3D12CommandAllocator* m_ComputeCmdAllocator = nullptr;
        ID3D12CommandAllocator* m_CopyCmdAllocator = nullptr;

        FSwapchainImpl m_Swapchain;
        FQueueImpl m_RenderQueue;
        FQueueImpl m_ComputeQueue;
        FQueueImpl m_CopyQueue;
    };
}
