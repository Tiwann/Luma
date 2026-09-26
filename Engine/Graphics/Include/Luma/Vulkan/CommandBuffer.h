#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class Device;

    class LUMA_GRAPHICS_API CommandBuffer : public RHI::CommandBuffer
    {
    public:
        QueueType getCommandBufferType() override;
        bool initialize(const RHI::CommandBufferDesc& cmdBufferDesc) override;
        void destroy() override;
        void reset() override;
        bool begin() override;
        void end() override;

        void beginDebugGroup(StringView name, const Color& color) override;
        void endDebugGroup() override;

        void clearColorTarget(uint32_t targetIndex, const Color& color) override;
        void clearDepthStencilTarget(float depth, uint8_t stencil) override;
        void clearColorTexture(RHI::Texture* texture, const Color& color, const TextureSubresourceRange& subresourceRange) override;
        void clearColorTexture(RHI::Texture* texture, const Color& color) override;
        void bindVertexBuffers(ArrayView<VertexBufferBinding> bindings) override;
        void bindIndexBuffer(const RHI::Buffer* buffer, int64_t offset, IndexFormat format) override;
        void bindRenderPipeline(const RHI::RenderPipeline* pipeline) override;
        void pushConstants(const RHI::Shader* shader, ShaderStageFlags stageFlags, const void* data, uint64_t offset, uint64_t size) override;
        void beginRenderPass(const RHI::RenderPassDesc& renderPassDesc) override;
        void endRenderPass() override;
        void setViewports(const Array<Viewport>& viewports) override;
        void setViewport(const Viewport& viewport) override;
        void setScissors(const Array<Scissor>& scissors) override;
        void setScissor(const Scissor& scissor) override;
        void draw(const DrawCommand& drawCmd) override;
        void drawIndexed(const DrawIndexedCommand& drawIndexedCmd) override;
        void drawIndirect(const RHI::Buffer* buffer, uint64_t offset, uint32_t drawCount) override;
        void drawIndexedIndirect(const RHI::Buffer* buffer, uint64_t offset, uint32_t drawCount) override;
        void bindComputePipeline(const RHI::ComputePipeline* pipeline) override;
        void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override;
        void dispatchIndirect(RHI::Buffer* buffer, int64_t offset) override;
        void copyBuffer(RHI::Buffer* srcBuffer, RHI::Buffer* dstBuffer, int64_t srcOffset, int64_t dstOffset, uint64_t size) override;
        void copyBufferToTexture(RHI::Buffer* buffer, int64_t offset, uint64_t size, RHI::Texture* texture, uint32_t arrayIndex,uint32_t mipLevel) override;
        void blitTexture(const RHI::Texture* srcTexture, const FRect3u& srcRect, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t
                         srcArrayCount, const RHI::Texture* destTexture, const
                         FRect3u& destRect, const TextureSubresourceRange& destRange, uint32_t destBaseArrayLayer, uint32_t destArrayCount, Filter
                         filter, uint32_t destMipLevel) override;
        void textureBarriers(ArrayView<TextureBarrier> barriers) override;
        void bufferBarriers(ArrayView<BufferBarrier> barriers) override;

        void bindBindingGroup(const RHI::BindingGroup* bindingGroup) override;
        void bindDescriptorBuffer(const RHI::Buffer* buffer) override;
        void setName(StringView name) override;

        VkCommandBuffer getHandle() const { return m_Handle; }
        VkCommandPool getPool() const { return m_PoolHandle; }
    private:
        Device* m_Device = nullptr;
        QueueType m_CmdBufferType = QueueType::None;
        VkCommandBuffer m_Handle = nullptr;
        VkCommandPool m_PoolHandle = nullptr;
    };
}
