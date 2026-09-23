#pragma once
#include <cstdint>
#include "IndexFormat.h"
#include "TextureSubresourceRange.h"
#include "Luma/Math/Color.h"

namespace Luma
{
    struct FScissor;
    struct FViewport;
    struct FRenderPassDesc;
    struct IComputePipeline;
    struct IRenderPipeline;
    struct ITexture;
    struct IBuffer;
    struct IBindingGroup;

    struct FDrawCommand
    {
        uint32_t vertexCount;
        uint32_t instanceCount;
        uint32_t firstVertex;
        uint32_t firstInstance;
    };

    struct FDrawIndexedCommand
    {
        uint32_t indexCount;
        uint32_t instanceCount;
        uint32_t firstIndex;
        int32_t vertexOffset;
        uint32_t firstInstance;
    };

    struct FDrawIndirectCommand
    {
        const IBuffer* buffer;
        int64_t offset;
        uint32_t drawCount;
    };

    struct FDrawIndexedIndirectCommand
    {
        const IBuffer* buffer;
        int64_t offset;
        uint32_t drawCount;
    };

    struct FClearColorTargetCommand
    {
        uint32_t targetIndex;
        FColor color;
        FRect2u area;
        uint32_t startLayer;
        uint32_t layerCount;
    };

    struct FClearDepthStencilTargetCommand
    {
        FRect2u area;
        float depth;
        uint8_t stencil;
    };

    struct FClearColorTextureCommand
    {
        const ITexture* texture;
        FColor color;
        FTextureSubresourceRange subresource;
    };

    struct FClearDepthStencilTextureCommand
    {
        const ITexture* texture;
        float depth;
        uint8_t stencil;
        FTextureSubresourceRange subresource;
    };

    struct FVertexBufferBinding
    {
        const IBuffer* buffer;
        int64_t offset;
    };

    struct FBindVertexBuffersCommand
    {
        const FVertexBufferBinding* bindings;
        uint32_t bindingCount;
    };

    struct FBindIndexBufferCommand
    {
        const IBuffer* buffer;
        int64_t offset;
        uint64_t size;
        EIndexFormat format;
    };

    struct FBindRenderPipelineCommand
    {
        const IRenderPipeline* pipeline;
    };

    struct FBindComputePipelineCommand
    {
        const IComputePipeline* pipeline;
    };

    struct FBeginRenderPassCommand
    {
        const FRenderPassDesc* renderPassDesc;
    };

    struct FEndRenderPassCommand
    {
        uint32_t dummy;
    };

    struct FSetViewportsCommand
    {
        const FViewport* viewports;
        uint32_t viewportsCount;
    };

    struct FSetScissorsCommand
    {
        const FScissor* scissors;
        uint32_t scissorsCount;
    };

    struct FDispatchCommand
    {
        uint32_t groupCountX;
        uint32_t groupCountY;
        uint32_t groupCountZ;
    };

    struct FDispatchIndirectCommand
    {
        const IBuffer* buffer;
        int64_t offset;
    };

    struct FBindBindingGroupCommand
    {
        const IBindingGroup* bindingGroup;
    };

    enum class EGPUCommandType
    {
        Draw,
        DrawIndexed,
        DrawIndirect,
        DrawIndexedIndirect,
        ClearColorTarget,
        ClearDepthStencilTarget,
        ClearColorTexture,
        ClearDepthStencilTexture,
        BindVertexBuffers,
        BindIndexBuffer,
        BindRenderPipeline,
        BindComputePipeline,
        BeginRenderPass,
        EndRenderPass,
        SetViewports,
        SetScissors,
        Dispatch,
        DispatchIndirect,
        BindBindingGroup,
    };

    struct FGPUCommand
    {
        EGPUCommandType type;

        union
        {
            FDrawCommand draw;
            FDrawIndexedCommand drawIndexed;
            FDrawIndirectCommand drawIndirect;
            FDrawIndexedIndirectCommand drawIndexedIndirect;
            FClearColorTargetCommand clearColorTarget;
            FClearDepthStencilTargetCommand clearDepthStencilTarget;
            FClearColorTextureCommand clearColorTexture;
            FClearDepthStencilTextureCommand clearDepthStencilTexture;
            FBindVertexBuffersCommand bindVertexBuffers;
            FBindIndexBufferCommand bindIndexBuffer;
            FBindRenderPipelineCommand bindRenderPipeline;
            FBindComputePipelineCommand bindComputePipeline;
            FBeginRenderPassCommand beginRenderPass;
            FEndRenderPassCommand endRenderPass;
            FSetViewportsCommand setViewports;
            FSetScissorsCommand setScissors;
            FDispatchCommand dispatch;
            FDispatchIndirectCommand dispatchIndirect;
            FBindBindingGroupCommand bindBindingGroup;
        };
    };

    static_assert(std::is_trivially_copyable_v<FGPUCommand>);
}
