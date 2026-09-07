#pragma once
#include "Luma/Rendering/GpuDevice.h"

namespace Luma::OpenGL
{
    constexpr bool isHandleValid(uint32_t handle) { return handle != 0xFFFFFFFF; }

    class FGpuDeviceImpl : public IGpuDevice
    {
    public:
        EGpuDeviceType getDeviceType() override;
        bool initialize(const FGpuDeviceDesc& deviceDesc) override;

        void destroy() override;
        bool beginFrame() override;
        void endFrame() override;
        void present() override;
        void waitIdle() override;
        uint32_t getTextureCount() const override;
        uint32_t getFrameIndex() const override;
        bool hasVSync() override;
        void setVSync(bool enabled) override;

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
        ITextureView* getAcquiredSwapchainTextureView() override;

    private:
        IWindow* m_Window = nullptr;
    };
}
