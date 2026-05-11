#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class LUMA_GRAPHICS_API FCommandBufferImpl : public ICommandBuffer
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

        void clearColor(uint32_t attachmentIndex, const FColor& color) override;
        void clearDepthStencil(float depth, uint8_t stencil) override;
        void clearColorTexture(ITexture* texture, const FColor& color, const FTextureSubresourceRange& subresourceRange) override;
        void clearColorTexture(ITexture* texture, const FColor& color) override;
        void bindVertexBuffer(const IBuffer* buffer, int64_t offset) override;
        void bindIndexBuffer(const IBuffer* buffer, uint64_t offset, EIndexFormat format) override;
        void bindGraphicsPipeline(const IGraphicsPipeline* pipeline) override;
        void pushConstants(const IShader* shader, FShaderStageFlags stageFlags, const void* data, uint64_t offset, uint64_t size) override;
        void beginRenderPass(const FRenderPassDesc& renderPassDesc) override;
        void endRenderPass() override;
        void setViewport(const FViewport& viewport) override;
        void setScissor(const FScissor& scissor) override;
        void draw(const FDrawCommand& drawCmd) override;
        void drawIndexed(const FDrawIndexedCommand& drawIndexedCmd) override;
        void drawIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount) override;
        void drawIndexedIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount) override;
        void bindComputePipeline(const IComputePipeline* pipeline) override;
        void bindMaterial(const FMaterial* material) override;
        void bindBindingSet(const IBindingSet* bindingSet, const IShader* shader) override;
        void drawStaticMesh(const FStaticMesh* staticMesh, const FMaterial* material, const FMatrix4f& transform, const FCamera& camera) override;
        void drawStaticMesh(const FStaticMesh* staticMesh, const FMatrix4f& transform, const FCamera& camera) override;
        void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override;
        void dispatchIndirect(IBuffer* buffer, int64_t offset) override;
        void copyBuffer(IBuffer* srcBuffer, IBuffer* dstBuffer, int64_t srcOffset, int64_t dstOffset, uint64_t size) override;
        void copyBufferToTexture(IBuffer* buffer, int64_t offset, uint64_t size, ITexture* texture, uint32_t arrayIndex,uint32_t mipLevel) override;
        void textureBarrier(const FTextureBarrier& barrier) override;
        void bufferBarrier(const FBufferBarrier& barrier) override;
        void setName(FStringView name) override;
        VkCommandBuffer getHandle() const { return m_Handle; }

    private:
        FRenderDeviceImpl* m_Device = nullptr;
        EQueueType m_CmdBufferType = EQueueType::Render;
        VkCommandBuffer m_Handle = nullptr;
        VkCommandPool m_PoolHandle = nullptr;
    };
}
