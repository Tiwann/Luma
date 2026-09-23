#pragma once
#include "BufferUsage.h"
#include "DeviceType.h"
#include "Sampler.h"
#include "SwpchainBuffering.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Graphics/Export.h"
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct Window;
}

namespace Luma::RHI
{
    struct FenceDesc;
    struct Fence;
    struct SamplerDesc;
    struct Sampler;
    struct TextureView;
    struct Texture;
    struct Swapchain;
    struct ComputePipelineDesc;
    struct ComputePipeline;
    struct RenderPipelineDesc;
    struct RenderPipeline;
    struct CommandBufferDesc;
    struct CommandBuffer;
    struct Shader;
    struct ShaderDesc;
    struct TextureViewDesc;
    struct TextureDesc;
    struct BufferDesc;
    struct Buffer;
    struct Queue;

    struct DeviceDesc
    {
        Window* window = nullptr;
        DeviceType deviceType = DeviceType::None;
        SwapchainBuffering buffering = SwapchainBuffering::None;
        bool vSync = false;
    };

    struct LUMA_GRAPHICS_API Device : RefCounted<Device>
    {
        ~Device() override = default;
        virtual DeviceType getDeviceType() = 0;
        virtual bool initialize(const DeviceDesc& deviceDesc) = 0;
        virtual void destroy() = 0;

        virtual bool beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual void present() = 0;
        virtual void waitIdle() = 0;
        virtual uint32_t getTextureCount() const = 0;
        virtual uint32_t getFrameIndex() const = 0;
        virtual bool hasVSync();
        virtual void setVSync(bool enabled);

        virtual Swapchain* getSwapchain() { return nullptr; }
        virtual Queue* getRenderQueue() { return nullptr; }
        virtual Queue* getComputeQueue() { return nullptr; }
        virtual Queue* getCopyQueue() { return nullptr; }

        virtual Buffer* createBuffer(const BufferDesc& bufferDesc) = 0;
        Buffer* createBuffer(BufferUsage usage, uint64_t size, bool alwaysMapped = false);

        virtual Texture* createTexture(const TextureDesc& textureDesc) = 0;
        virtual TextureView* createTextureView(const TextureViewDesc& textureViewDesc) = 0;
        virtual Shader* createShader(const ShaderDesc& shaderDesc) = 0;
        Shader* createShader(FStringView vertexPath, FStringView fragmentPath);
        virtual CommandBuffer* createCommandBuffer(const CommandBufferDesc& cmdBufferDesc) = 0;
        CommandBuffer* createCommandBuffer(Queue* queue);
        virtual CommandBuffer* getCommandBuffer() = 0;
        virtual Sampler* createSampler(const SamplerDesc& samplerDesc) = 0;
        Sampler* getOrCreateSampler(const SamplerDesc& samplerDesc);
        virtual RenderPipeline* createRenderPipeline(const RenderPipelineDesc& pipelineDesc) = 0;
        virtual ComputePipeline* createComputePipeline(const ComputePipelineDesc& pipelineDesc) = 0;
        virtual Fence* createFence(const FenceDesc& fenceDesc) = 0;
        Fence* createFence(uint64_t initialValue = 0);
        virtual TextureView* getAcquiredSwapchainTextureView() = 0;
        virtual Texture* getAcquiredSwapchainTexture() = 0;
    protected:
        THashMap<SamplerDesc, Sampler*, FSamplerDescHasher> m_PerDescSamplers;
    };

    LUMA_GRAPHICS_API Device* createDevice(const DeviceDesc& deviceDesc);
    LUMA_GRAPHICS_API Device* createDevice(Window* window, DeviceType deviceType = DeviceType::Auto, SwapchainBuffering buffering = SwapchainBuffering::TripleBuffering, bool vsync = true);
}
