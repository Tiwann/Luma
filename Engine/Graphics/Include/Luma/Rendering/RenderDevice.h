#pragma once
#include "Luma/Graphics/Export.h"
#include <cstdint>
#include <unordered_map>


namespace luma
{
    struct FBufferDesc;
    struct FTextureDesc;
    struct FShaderDesc;
    struct FCommandBufferDesc;
    struct FFenceDesc;
    struct FSamplerDesc;
    struct FGraphicsPipelineDesc;
    struct FComputePipelineDesc;


    struct IWindow;
    struct IBuffer;
    struct ITexture;
    struct IShader;
    struct ICommandBuffer;
    struct IRenderCommandBuffer;
    struct IComputeCommandBuffer;
    struct ICopyCommandBuffer;
    struct IFence;
    struct ISampler;
    struct IGraphicsPipeline;
    struct IComputePipeline;

    enum class ERenderDeviceType
    {
        None,
#ifdef LUMA_BUILD_VULKAN
        Vulkan,
#endif
#ifdef LUMA_BUILD_D3D12
        D3D12,
#endif
#ifdef LUMA_BUILD_OPENGL
        OpenGL,
#endif
#ifdef LUMA_BUILD_WEBGPU
        WebGPU,
#endif
#ifdef LUMA_BUILD_NVN
        NVN,
#endif
#ifdef LUMA_BUILD_DEKO3D
        Deko3D,
#endif
#ifdef LUMA_BUILD_GNM
        GNM,
#endif
    };

    struct FRenderDeviceDesc
    {
        IWindow* window = nullptr;
        ERenderDeviceType deviceType = ERenderDeviceType::None;
    };

    struct LUMA_GRAPHICS_API IRenderDevice
    {
        virtual ~IRenderDevice() = default;
        virtual ERenderDeviceType getDeviceType() = 0;
        virtual bool initialize(const FRenderDeviceDesc& deviceDesc) = 0;
        virtual void destroy() = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
        virtual void present() = 0;
        virtual void waitIdle() = 0;
        virtual uint32_t getFrameCount() = 0;
        virtual uint32_t getCurrentFrameIndex() = 0;
        virtual bool hasVSync() { return false; }

        virtual IBuffer* createBuffer(const FBufferDesc& bufferDesc) = 0;
        virtual ITexture* createTexture(const FTextureDesc& textureDesc) = 0;
        virtual IShader* createShader(const FShaderDesc& shaderDesc) = 0;
        virtual ICommandBuffer* createCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) = 0;
        virtual IRenderCommandBuffer* createRenderCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) = 0;
        virtual IComputeCommandBuffer* createComputeCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) = 0;
        virtual ICopyCommandBuffer* createCopyCommandBuffer(const FCommandBufferDesc& cmdBufferDesc) = 0;
        virtual ISampler* createSampler(const FSamplerDesc& samplerDesc) = 0;
        virtual IGraphicsPipeline* createGraphicsPipeline(const FGraphicsPipelineDesc& pipelineDesc) = 0;
        virtual IComputePipeline* createComputePipeline(const FComputePipelineDesc& pipelineDesc) = 0;
        virtual IFence* createFence(const FFenceDesc& fenceDesc) = 0;

        //using FSamplerMap = std::unordered_map<FSamplerDesc, ISampler*>;
        //FSamplerMap m_SamplerMap;
    };

    LUMA_GRAPHICS_API IRenderDevice* createRenderDevice(const FRenderDeviceDesc& deviceDesc);
}
