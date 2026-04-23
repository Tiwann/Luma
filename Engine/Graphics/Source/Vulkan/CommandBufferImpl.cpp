#include "CommandBufferImpl.h"
#include "RenderDeviceImpl.h"
#include "BufferImpl.h"
#include "Conversions.h"
#include "VulkanUtils.h"
#include <Volk/volk.h>


namespace Luma::Vulkan
{
    EQueueType FCommandBufferImpl::getCommandBufferType()
    {
        return m_CmdBufferType;
    }

    bool FCommandBufferImpl::initialize(const FCommandBufferDesc& cmdBufferDesc)
    {
        if (!cmdBufferDesc.device) return false;

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(cmdBufferDesc.device);
        const VkCommandPool commandPool = device->getCommandPool(cmdBufferDesc.queueType);
        if (!commandPool) return false;

        VkCommandBufferAllocateInfo info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
        info.commandPool = commandPool;
        info.commandBufferCount = 1;
        info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        const VkDevice deviceHandle = device->getHandle();
        if (VK_FAILED(vkAllocateCommandBuffers(deviceHandle, &info, &m_Handle)))
            return false;

        m_Device = device;
        m_PoolHandle = commandPool;
        return true;
    }

    void FCommandBufferImpl::destroy()
    {
        if (!m_Device) return;
        const VkDevice deviceHandle = m_Device->getHandle();
        vkFreeCommandBuffers(deviceHandle, m_PoolHandle, 1, &m_Handle);
        m_Device = nullptr;
        m_PoolHandle = nullptr;
        m_Handle = nullptr;
    }

    void FCommandBufferImpl::reset()
    {
        vkResetCommandBuffer(m_Handle, 0);
    }

    bool FCommandBufferImpl::begin()
    {
        VkCommandBufferBeginInfo info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        if (vkBeginCommandBuffer(m_Handle, &info) != VK_SUCCESS)
            return false;
        return true;
    }

    void FCommandBufferImpl::end()
    {
        vkEndCommandBuffer(m_Handle);
    }

    void FCommandBufferImpl::bindVertexBuffer(const IBuffer* buffer, const int64_t offset)
    {
        if (!buffer) return;
        const VkDeviceSize offsets[] { static_cast<VkDeviceSize>(offset) };
        const VkBuffer bufferImpl = static_cast<const FBufferImpl*>(buffer)->getHandle();
        const VkBuffer buffers[] { bufferImpl };
        vkCmdBindVertexBuffers(m_Handle, 0, 1, buffers, offsets);
    }

    void FCommandBufferImpl::bindIndexBuffer(const IBuffer* buffer, const uint64_t offset, const EIndexFormat format)
    {
        if (!buffer) return;
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        const VkBuffer bufferHandle = bufferImpl->getHandle();
        const VkDeviceSize size = bufferImpl->getSize();

        vkCmdBindIndexBuffer2(m_Handle, bufferHandle, offset, size, convert<VkIndexType>(format));
    }

    void FCommandBufferImpl::bindGraphicsPipeline(const IGraphicsPipeline* pipeline)
    {
    }

    void FCommandBufferImpl::setViewport(const FViewport& viewport)
    {
        VkViewport vp { };
        vp.x = viewport.area.x;
        vp.y = viewport.area.y + viewport.area.height;
        vp.width = viewport.area.width;
        vp.height = -viewport.area.height;
        vp.minDepth = viewport.minDepth;
        vp.maxDepth = viewport.maxDepth;
        vkCmdSetViewport(m_Handle, 0, 1, &vp);
    }

    void FCommandBufferImpl::setScissor(const FRect2u& scissor)
    {
        VkRect2D rect { };
        rect.offset.x = scissor.x;
        rect.offset.y = scissor.y;
        rect.extent.width = scissor.width;
        rect.extent.height = scissor.height;
        vkCmdSetScissor(m_Handle, 0, 1, &rect);
    }

    void FCommandBufferImpl::draw(const FDrawCommand& drawCmd)
    {
        vkCmdDraw(m_Handle,
            drawCmd.vertexCount,
            drawCmd.instanceCount,
            drawCmd.firstVertex,
            drawCmd.firstInstance);
    }

    void FCommandBufferImpl::drawIndexed(const FDrawIndexedCommand& drawIndexedCmd)
    {
        vkCmdDrawIndexed(m_Handle,
            drawIndexedCmd.indexCount,
            drawIndexedCmd.instanceCount,
            drawIndexedCmd.firstIndex,
            drawIndexedCmd.vertexOffset,
            drawIndexedCmd.firstInstance);
    }

    void FCommandBufferImpl::drawIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount)
    {
    }

    void FCommandBufferImpl::drawIndexedIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount)
    {
    }

    void FCommandBufferImpl::bindComputePipeline(const IComputePipeline* pipeline)
    {
    }

    void FCommandBufferImpl::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
    {
        vkCmdDispatch(m_Handle, groupCountX, groupCountY, groupCountZ);
    }

    void FCommandBufferImpl::dispatchIndirect(IBuffer* buffer, int64_t offset)
    {
    }

    void FCommandBufferImpl::copyBuffer(IBuffer* srcBuffer, int64_t srcOffset, uint64_t srcSize, IBuffer* dstBuffer,int64_t dstOffset, uint64_t dstSize)
    {
    }

    void FCommandBufferImpl::copyBufferToTexture(IBuffer* buffer, int64_t offset, uint64_t size, ITexture* texture,uint32_t arraySlice, uint32_t mipLevel)
    {
    }

    void FCommandBufferImpl::setName(FStringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_COMMAND_BUFFER, m_Handle, name);
    }
}
