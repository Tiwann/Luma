#pragma once
#include "../../../../Core/Include/Luma/Math/Vector3.h"
#include "../../../../Core/Include/Luma/Math/Rect2.h"
#include "IndexFormat.h"
#include <cstdint>
#include <string>

namespace luma
{
    struct IRenderDevice;
    struct IBuffer;
    struct IGraphicsPipeline;
    struct IComputePipeline;
    struct ITexture;

    enum class ECommandBufferType : uint32_t
    {
        Render,
        Compute,
        Copy
    };

    struct FCommandBufferDesc
    {
        IRenderDevice* device = nullptr;
        ECommandBufferType cmdBufferType = ECommandBufferType::Render;
    };

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

    struct ICommandBuffer
    {
        ICommandBuffer() = default;
        virtual ~ICommandBuffer() = default;
        virtual ECommandBufferType getCommandBufferType() = 0;
        virtual void begin() = 0;
        virtual void end() = 0;

        virtual void beginDebugGroup(std::string_view name){}
        virtual void endDebugGroup(){}
    };

    class IRenderCommandBuffer : ICommandBuffer
    {
    public:
        IRenderCommandBuffer() = default;
        ECommandBufferType getCommandBufferType() override { return ECommandBufferType::Render; }
        virtual void bindVertexBuffer(const IBuffer* buffer, int64_t offset) = 0;
        virtual void bindIndexBuffer(const IBuffer* buffer, uint64_t offset, EIndexFormat format) = 0;
        virtual void bindGraphicsPipeline(const IGraphicsPipeline* pipeline) = 0;
        virtual void setScissor(const FRect2u& scissor) = 0;
        virtual void setViewport(const FRect2f& viewport, float minDepth = 0.0f, float maxDepth = 1.0f) = 0;
        virtual void draw(const FDrawCommand& drawCmd) = 0;
        void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
        virtual void drawIndexed(const FDrawIndexedCommand& drawIndexedCmd) = 0;
        void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
        virtual void drawIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount) = 0;
        virtual void drawIndexedIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount) = 0;
    };

    class IComputeCommandBuffer : ICommandBuffer
    {
    public:
        IComputeCommandBuffer() = default;
        ECommandBufferType getCommandBufferType() override { return ECommandBufferType::Compute; }
        virtual void bindComputePipeline(const IComputePipeline* pipeline) = 0;
        virtual void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
        virtual void dispatchIndirect(IBuffer* buffer, int64_t offset) = 0;
        void dispatch(const FVector3u& groupCounts) { dispatch(groupCounts.x, groupCounts.y, groupCounts.z); }
    };

    class ICopyCommandBuffer : ICommandBuffer
    {
        public:
        ICopyCommandBuffer() = default;
        ECommandBufferType getCommandBufferType() override { return ECommandBufferType::Copy; }
        virtual void copyBuffer(IBuffer* srcBuffer, int64_t srcOffset, uint64_t srcSize, IBuffer* dstBuffer, int64_t dstOffset, uint64_t dstSize) = 0;
        virtual void copyBufferToTexture(IBuffer* buffer, int64_t offset, uint64_t size, ITexture* texture, uint32_t arraySlice, uint32_t mipLevel) = 0;
    };
}
