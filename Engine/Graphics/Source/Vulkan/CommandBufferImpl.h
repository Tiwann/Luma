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

        void bindVertexBuffer(const IBuffer* buffer, int64_t offset) override;
        void bindIndexBuffer(const IBuffer* buffer, uint64_t offset, EIndexFormat format) override;
        void bindGraphicsPipeline(const IGraphicsPipeline* pipeline) override;
        void setViewport(const FViewport& viewport) override;
        void setScissor(const FRect2u& scissor) override;
        void draw(const FDrawCommand& drawCmd) override;
        void drawIndexed(const FDrawIndexedCommand& drawIndexedCmd) override;
        void drawIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount) override;
        void drawIndexedIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount) override;
        void bindComputePipeline(const IComputePipeline* pipeline) override;
        void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override;
        void dispatchIndirect(IBuffer* buffer, int64_t offset) override;
        void copyBuffer(IBuffer* srcBuffer, int64_t srcOffset, uint64_t srcSize, IBuffer* dstBuffer, int64_t dstOffset,uint64_t dstSize) override;
        void copyBufferToTexture(IBuffer* buffer, int64_t offset, uint64_t size, ITexture* texture, uint32_t arraySlice,uint32_t mipLevel) override;

        void setName(FStringView name) override;
        VkCommandBuffer getHandle() const { return m_Handle; }
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        EQueueType m_CmdBufferType = EQueueType::Render;
        VkCommandBuffer m_Handle = nullptr;
        VkCommandPool m_PoolHandle = nullptr;
    };
}
