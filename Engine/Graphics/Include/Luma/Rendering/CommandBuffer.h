#pragma once
#include "Filter.h"
#include "QueueType.h"
#include "ResourceBarrier.h"
#include "Scissor.h"
#include "ShaderStage.h"
#include "TextureSubresourceRange.h"
#include "Viewport.h"
#include "Luma/Containers/ArrayView.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Math/Color.h"
#include "Luma/Math/Rect2.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Rendering/Commands.h"

namespace Luma::RHI
{
    struct Device;
    struct Queue;
    struct Texture;
    struct Shader;
    struct BindingGroup;
    struct RenderPipeline;
    struct ComputePipeline;
    struct RenderPassDesc;

    struct CommandBufferDesc
    {
        Device* device = nullptr;
        Queue* queue = nullptr;
    };

    struct CommandBuffer : RefCounted<CommandBuffer>
    {
        CommandBuffer() = default;
        ~CommandBuffer() override = default;
        virtual QueueType getCommandBufferType() = 0;
        virtual bool initialize(const CommandBufferDesc& cmdBufferDesc) = 0;
        virtual void destroy() = 0;
        virtual void reset() = 0;
        virtual bool begin() = 0;
        virtual void end() = 0;

        virtual void beginDebugGroup(StringView name, const Color& color){}
        virtual void endDebugGroup(){}
        virtual void setName(StringView name) {}

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// RENDER CMDS
        ///////////////////////////////////////////////////////////////////////////////////////////////
        virtual void clearColorTarget(uint32_t targetIndex, const Color& color) = 0;
        virtual void clearDepthStencilTarget(float depth, uint8_t stencil) = 0;
        virtual void clearColorTexture(Texture* texture, const Color& color, const TextureSubresourceRange& subresourceRange) = 0;
        virtual void clearColorTexture(Texture* texture, const Color& color) = 0;
        virtual void bindVertexBuffers(ArrayView<VertexBufferBinding> bindings) = 0;
        void bindVertexBuffer(const Buffer* buffer, int64_t offset);
        virtual void bindIndexBuffer(const Buffer* buffer, int64_t offset, IndexFormat format) = 0;
        virtual void pushConstants(const Shader* shader, ShaderStageFlags stageFlags, const void* data, uint64_t offset, uint64_t size) = 0;
        template<typename T>
        void pushConstant(const Shader* shader, ShaderStageFlags stageFlags, const T& data, uint64_t offset = 0)
        {
            pushConstants(shader, stageFlags, &data, offset, sizeof(T));
        }
        virtual void bindRenderPipeline(const RenderPipeline* pipeline) = 0;
        virtual void beginRenderPass(const RenderPassDesc& renderPassDesc) = 0;
        virtual void endRenderPass() = 0;
        virtual void setScissors(const Array<Scissor>& scissors) = 0;
        virtual void setScissor(const Scissor& scissor) = 0;
        virtual void setViewports(const Array<Viewport>& viewports) = 0;
        virtual void setViewport(const Viewport& viewport) = 0;
        virtual void draw(const DrawCommand& drawCmd) = 0;
        void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
        virtual void drawIndexed(const DrawIndexedCommand& drawIndexedCmd) = 0;
        void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
        virtual void drawIndirect(const Buffer* buffer, uint64_t offset, uint32_t drawCount) = 0;
        virtual void drawIndexedIndirect(const Buffer* buffer, uint64_t offset, uint32_t drawCount) = 0;
        virtual void textureBarriers(ArrayView<TextureBarrier> barriers) = 0;
        virtual void bufferBarriers(ArrayView<BufferBarrier> barriers) = 0;
        virtual void bindBindingGroup(const BindingGroup* bindingGroup) = 0;
        virtual void bindDescriptorBuffer(const Buffer* buffer){LUMA_ASSERT(false, "Not Implemented");}
        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// COMPUTE CMDS
        ///////////////////////////////////////////////////////////////////////////////////////////////
        virtual void bindComputePipeline(const ComputePipeline* pipeline) = 0;
        virtual void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
        void dispatch(const FVector3u& groupCounts) { dispatch(groupCounts.x, groupCounts.y, groupCounts.z); }
        virtual void dispatchIndirect(Buffer* buffer, int64_t offset) = 0;


        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// COPY CMDS
        ///////////////////////////////////////////////////////////////////////////////////////////////
        virtual void copyBuffer(Buffer* srcBuffer, Buffer* dstBuffer, int64_t srcOffset, int64_t dstOffset, uint64_t size) = 0;
        virtual void copyBufferToTexture(Buffer* buffer, int64_t offset, uint64_t size, Texture* texture, uint32_t arraySlice, uint32_t mipLevel) = 0;
        virtual void blitTexture(const Texture* srcTexture, const FRect3u& srcRect, uint32_t srcMip, uint32_t srcBaseArrayLayer, uint32_t srcArrayCount, const Texture* destTexture, const
                                 FRect3u& destRect, const TextureSubresourceRange& destRange, uint32_t destBaseArrayLayer, uint32_t destArrayCount, Filter
                                 filter, uint32_t destMipLevel) = 0;
    protected:
        const RenderPassDesc* m_CurrentRenderPassDesc = nullptr;
    };
}
