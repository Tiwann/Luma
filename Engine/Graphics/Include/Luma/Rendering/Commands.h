#pragma once
#include <cstdint>
#include "IndexFormat.h"
#include "RenderPassDesc.h"
#include "TextureSubresourceRange.h"
#include "Luma/Math/Color.h"

namespace Luma
{
    namespace RHI
    {
        struct BindingGroup;
        struct ComputePipeline;
        struct RenderPipeline;
    }

    struct DrawCommand
    {
        uint32_t vertexCount;
        uint32_t instanceCount;
        uint32_t firstVertex;
        uint32_t firstInstance;
    };

    struct DrawIndexedCommand
    {
        uint32_t indexCount;
        uint32_t instanceCount;
        uint32_t firstIndex;
        int32_t vertexOffset;
        uint32_t firstInstance;
    };

    struct DrawIndirectCommand
    {
        const RHI::Buffer* buffer;
        int64_t offset;
        uint32_t drawCount;
    };

    struct DrawIndexedIndirectCommand
    {
        const RHI::Buffer* buffer;
        int64_t offset;
        uint32_t drawCount;
    };

    struct ClearColorTargetCommand
    {
        uint32_t targetIndex;
        Color color;
        FRect2u area;
        uint32_t startLayer;
        uint32_t layerCount;
    };

    struct ClearDepthStencilTargetCommand
    {
        FRect2u area;
        float depth;
        uint8_t stencil;
    };

    struct ClearColorTextureCommand
    {
        const RHI::Texture* texture;
        Color color;
        TextureSubresourceRange subresource;
    };

    struct ClearDepthStencilTextureCommand
    {
        const RHI::Texture* texture;
        float depth;
        uint8_t stencil;
        TextureSubresourceRange subresource;
    };

    struct VertexBufferBinding
    {
        const RHI::Buffer* buffer;
        int64_t offset;
    };

    struct BindVertexBuffersCommand
    {
        const VertexBufferBinding* bindings;
        uint32_t bindingCount;
    };

    struct BindIndexBufferCommand
    {
        const RHI::Buffer* buffer;
        int64_t offset;
        uint64_t size;
        IndexFormat format;
    };

    struct BindRenderPipelineCommand
    {
        const RHI::RenderPipeline* pipeline;
    };

    struct BindComputePipelineCommand
    {
        const RHI::ComputePipeline* pipeline;
    };

    struct BeginRenderPassCommand
    {
        const RHI::RenderPassDesc* renderPassDesc;
    };

    struct EndRenderPassCommand
    {
        uint32_t dummy;
    };

    struct SetViewportsCommand
    {
        const Viewport* viewports;
        uint32_t viewportsCount;
    };

    struct SetScissorsCommand
    {
        const Scissor* scissors;
        uint32_t scissorsCount;
    };

    struct DispatchCommand
    {
        uint32_t groupCountX;
        uint32_t groupCountY;
        uint32_t groupCountZ;
    };

    struct DispatchIndirectCommand
    {
        const RHI::Buffer* buffer;
        int64_t offset;
    };

    struct BindBindingGroupCommand
    {
        const RHI::BindingGroup* bindingGroup;
    };

    enum class GPUCommandType
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

    struct GPUCommand
    {
        GPUCommandType type;

        union
        {
            DrawCommand draw;
            DrawIndexedCommand drawIndexed;
            DrawIndirectCommand drawIndirect;
            DrawIndexedIndirectCommand drawIndexedIndirect;
            ClearColorTargetCommand clearColorTarget;
            ClearDepthStencilTargetCommand clearDepthStencilTarget;
            ClearColorTextureCommand clearColorTexture;
            ClearDepthStencilTextureCommand clearDepthStencilTexture;
            BindVertexBuffersCommand bindVertexBuffers;
            BindIndexBufferCommand bindIndexBuffer;
            BindRenderPipelineCommand bindRenderPipeline;
            BindComputePipelineCommand bindComputePipeline;
            BeginRenderPassCommand beginRenderPass;
            EndRenderPassCommand endRenderPass;
            SetViewportsCommand setViewports;
            SetScissorsCommand setScissors;
            DispatchCommand dispatch;
            DispatchIndirectCommand dispatchIndirect;
            BindBindingGroupCommand bindBindingGroup;
        };
    };

    static_assert(std::is_trivially_copyable_v<GPUCommand>);
}
