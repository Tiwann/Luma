#pragma once
#include "Luma/Rendering/CommandBuffer.h"
#include "D3D12Fwd.h"


namespace Luma::D3D12
{
    class FGpuDeviceImpl;

    class FCommandBufferImpl : public ICommandBuffer
    {
    public:
        EQueueType getCommandBufferType() override;
        bool initialize(const FCommandBufferDesc& cmdBufferDesc) override;
        void destroy() override;
        void reset() override;
        bool begin() override;
        void end() override;
        void beginDebugGroup(FStringView name, const FColor& color) override;
        void endDebugGroup() override;
        void setName(FStringView name) override;
        void clearColor(uint32_t attachmentIndex, const FColor& color) override;
        void clearDepthStencil(float depth, uint8_t stencil) override;
        void clearColorTexture(ITexture* texture, const FColor& color,const FTextureSubresourceRange& subresourceRange) override;
        void clearColorTexture(ITexture* texture, const FColor& color) override;
        void bindVertexBuffer(const IBuffer* buffer, int64_t offset) override;
        void bindIndexBuffer(const IBuffer* buffer, int64_t offset, EIndexFormat format) override;
        void pushConstants(const IShaderProgram* shader, FShaderStageFlags stageFlags, const void* data,uint64_t offset, uint64_t size) override;
        void bindRenderPipeline(const IRenderPipeline* pipeline) override;
        void beginRenderPass(const FRenderPassDesc& renderPassDesc) override;
        void endRenderPass() override;
        void setScissors(const TArray<FScissor>& scissors) override;
        void setScissor(const FScissor& scissor) override;
        void setViewports(const TArray<FViewport>& viewports) override;
        void setViewport(const FViewport& viewport) override;
        void draw(const FDrawCommand& drawCmd) override;
        void drawIndexed(const FDrawIndexedCommand& drawIndexedCmd) override;
        void drawIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount) override;
        void drawIndexedIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount) override;
        void bindMaterial(const FMaterial* material) override;
        void bindBindingSet(const IBindingSet* bindingSet, const IShaderProgram* shader) override;
        void textureBarrier(const FTextureBarrier& barrier) override;
        void bufferBarrier(const FBufferBarrier& barrier) override;
        void bindDescriptorBuffer(const IBuffer* buffer) override;
        void bindComputePipeline(const IComputePipeline* pipeline) override;
        void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override;
        void dispatchIndirect(IBuffer* buffer, int64_t offset) override;
        void copyBuffer(IBuffer* srcBuffer, IBuffer* dstBuffer, int64_t srcOffset, int64_t dstOffset,uint64_t size) override;
        void copyBufferToTexture(IBuffer* buffer, int64_t offset, uint64_t size, ITexture* texture, uint32_t arraySlice,uint32_t mipLevel) override;

        FGpuDeviceImpl* getDevice() const { return m_Device; }
        ID3D12GraphicsCommandList10* getHandle() const { return m_Handle; }
    private:
        FGpuDeviceImpl* m_Device = nullptr;
        ID3D12GraphicsCommandList10* m_Handle = nullptr;
        EQueueType m_QueueType = EQueueType::None;
    };
}
