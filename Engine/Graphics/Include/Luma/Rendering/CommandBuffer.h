#pragma once
#include "Luma/Memory/RefCounted.h"
#include "Luma/Graphics/Export.h"
#include "Luma/Math/Color.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Rect2.h"
#include "Luma/Containers/StringView.h"
#include "IndexFormat.h"
#include "QueueType.h"
#include <cstdint>


namespace Luma
{
    struct IRenderDevice;
    struct IBuffer;
    struct IGraphicsPipeline;
    struct IComputePipeline;
    struct ITexture;


    struct FCommandBufferDesc
    {
        IRenderDevice* device = nullptr;
        EQueueType queueType = EQueueType::Render;
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

    struct ICommandBuffer : IRefCounted<ICommandBuffer>
    {
        ICommandBuffer() = default;
        ~ICommandBuffer() override = default;
        virtual EQueueType getCommandBufferType() = 0;
        virtual bool initialize(const FCommandBufferDesc& cmdBufferDesc) = 0;
        virtual void destroy() = 0;
        virtual void reset() = 0;
        virtual bool begin() = 0;
        virtual void end() = 0;

        virtual void beginDebugGroup(FStringView name, const FColor& color){}
        virtual void endDebugGroup(){}

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// RENDER CMDS
        ///////////////////////////////////////////////////////////////////////////////////////////////
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

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// COMPUTE CMDS
        ///////////////////////////////////////////////////////////////////////////////////////////////
        virtual void bindComputePipeline(const IComputePipeline* pipeline) = 0;
        virtual void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
        void dispatch(const FVector3u& groupCounts) { dispatch(groupCounts.x, groupCounts.y, groupCounts.z); }
        virtual void dispatchIndirect(IBuffer* buffer, int64_t offset) = 0;


        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// COPY CMDS
        ///////////////////////////////////////////////////////////////////////////////////////////////
        virtual void copyBuffer(IBuffer* srcBuffer, int64_t srcOffset, uint64_t srcSize, IBuffer* dstBuffer, int64_t dstOffset, uint64_t dstSize) = 0;
        virtual void copyBufferToTexture(IBuffer* buffer, int64_t offset, uint64_t size, ITexture* texture, uint32_t arraySlice, uint32_t mipLevel) = 0;
    };

    class LUMA_GRAPHICS_API FRenderCommandBuffer
    {
    public:
        FRenderCommandBuffer() = delete;
        FRenderCommandBuffer(ICommandBuffer* cmdBuffer) : m_CmdBuffer(cmdBuffer){}
        virtual ~FRenderCommandBuffer() = default;

        void bindVertexBuffer(const IBuffer* buffer, int64_t offset);
        void bindIndexBuffer(const IBuffer* buffer, uint64_t offset, EIndexFormat format);
        void bindGraphicsPipeline(const IGraphicsPipeline* pipeline);
        void setScissor(const FRect2u& scissor);
        void setViewport(const FRect2f& viewport, float minDepth = 0.0f, float maxDepth = 1.0f);
        void draw(const FDrawCommand& drawCmd);
        void drawIndexed(const FDrawIndexedCommand& drawIndexedCmd);
        void drawIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount);
        void drawIndexedIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount);
    private:
        ICommandBuffer* m_CmdBuffer = nullptr;
    };
}
