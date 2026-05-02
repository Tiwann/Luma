#pragma once
#include "Luma/Memory/RefCounted.h"
#include "Luma/Math/Color.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Matrix.h"
#include "Luma/Containers/StringView.h"
#include "IndexFormat.h"
#include "QueueType.h"
#include "Scissor.h"
#include "Viewport.h"
#include "Camera.h"
#include <cstdint>

#include "ShaderStage.h"


namespace Luma
{
    class FStaticMesh;
    struct IShader;
    struct IBindingSet;
    class FMaterial;
    struct FTextureSubresourceRange;
    struct FBufferBarrier;
    struct FTextureBarrier;
    struct IRenderDevice;
    struct IBuffer;
    struct IGraphicsPipeline;
    struct IComputePipeline;
    struct ITexture;
    struct FRenderPassDesc;


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
        virtual void setName(FStringView name) {}

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// RENDER CMDS
        ///////////////////////////////////////////////////////////////////////////////////////////////
        virtual void clearColor(uint32_t attachmentIndex, const FColor& color) = 0;
        virtual void clearDepthStencil(float depth, uint8_t stencil) = 0;
        virtual void clearColorTexture(ITexture* texture, const FColor& color, const FTextureSubresourceRange& subresourceRange) = 0;
        virtual void clearColorTexture(ITexture* texture, const FColor& color) = 0;
        virtual void bindVertexBuffer(const IBuffer* buffer, int64_t offset) = 0;
        virtual void bindIndexBuffer(const IBuffer* buffer, uint64_t offset, EIndexFormat format) = 0;
        virtual void pushConstants(const IShader* shader, FShaderStageFlags stageFlags, const void* data, uint64_t offset, uint64_t size) = 0;
        virtual void bindGraphicsPipeline(const IGraphicsPipeline* pipeline) = 0;
        virtual void beginRenderPass(const FRenderPassDesc& renderPassDesc) = 0;
        virtual void endRenderPass() = 0;
        virtual void setScissor(const FScissor& scissor) = 0;
        virtual void setViewport(const FViewport& viewport) = 0;
        virtual void draw(const FDrawCommand& drawCmd) = 0;
        void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
        virtual void drawIndexed(const FDrawIndexedCommand& drawIndexedCmd) = 0;
        void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
        virtual void drawIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount) = 0;
        virtual void drawIndexedIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount) = 0;
        virtual void bindMaterial(const FMaterial* material) = 0;
        virtual void bindBindingSet(const IBindingSet* bindingSet, const IShader* shader) = 0;
        virtual void drawStaticMesh(const FStaticMesh* staticMesh, const FMaterial* material, const FMatrix4f& transform, const FCamera& camera) = 0;
        virtual void drawStaticMesh(const FStaticMesh* staticMesh, const FMatrix4f& transform, const FCamera& camera) = 0;
        virtual void textureBarrier(const FTextureBarrier& barrier) = 0;
        virtual void bufferBarrier(const FBufferBarrier& barrier) = 0;
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
        virtual void copyBuffer(IBuffer* srcBuffer, IBuffer* dstBuffer, int64_t srcOffset, int64_t dstOffset, uint64_t size) = 0;
        virtual void copyBufferToTexture(IBuffer* buffer, int64_t offset, uint64_t size, ITexture* texture, uint32_t arraySlice, uint32_t mipLevel) = 0;
    protected:
        const FRenderPassDesc* m_CurrentRenderPassDesc = nullptr;
    };
}
