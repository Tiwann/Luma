#include "Luma/Deko3D/CommandBufferImpl.h"
#include "Luma/Deko3D/GpuDeviceImpl.h"
#include "Luma/Deko3D/BufferImpl.h"
#include "Luma/Deko3D/Conversions.h"
#include "Luma/Memory/Memory.h"
#include <deko3d.h>


namespace Luma::Deko3d
{
    static constexpr uint32_t CMDMEMSIZE = Memory::KB(16);

    EQueueType FCommandBufferImpl::getCommandBufferType()
    {
        return m_CmdBufferType;
    }

    bool FCommandBufferImpl::initialize(const FCommandBufferDesc& cmdBufferDesc)
    {
        if (!cmdBufferDesc.device) return false;
        if (cmdBufferDesc.queueType == EQueueType::None) return false;

        FGpuDeviceImpl* device = static_cast<FGpuDeviceImpl*>(cmdBufferDesc.device);
        const DkDevice deviceHandle = device->getHandle();

        DkCmdBufMaker maker;
        dkCmdBufMakerDefaults(&maker, deviceHandle);
        maker.userData = this;
        maker.cbAddMem = [](void* userData, DkCmdBuf cmdbuf, size_t minReqSize)
        {

        };

        m_Handle = dkCmdBufCreate(&maker);
        dkCmdBufAddMemory(m_Handle, m_MemBlock, 0, CMDMEMSIZE);
        if (!m_Handle) return false;
        return true;
    }

    void FCommandBufferImpl::destroy()
    {
        dkCmdBufDestroy(m_Handle);
        m_Handle = nullptr;
        m_CmdBufferType = EQueueType::None;
    }

    void FCommandBufferImpl::reset()
    {
        dkCmdBufClear(m_Handle);
    }

    bool FCommandBufferImpl::begin()
    {
        return true;
    }

    void FCommandBufferImpl::end()
    {
        m_ListHandle = dkCmdBufFinishList(m_Handle);
    }

    void FCommandBufferImpl::beginDebugGroup(const FStringView name, const FColor& color)
    {
        ICommandBuffer::beginDebugGroup(name, color);
    }

    void FCommandBufferImpl::endDebugGroup()
    {
        ICommandBuffer::endDebugGroup();
    }

    void FCommandBufferImpl::clearColor(uint32_t attachmentIndex, const FColor& color)
    {
        dkCmdBufClearColorFloat(m_Handle, 0, 0xFFFFFFFF, color.r, color.g, color.b, color.a);
    }

    void FCommandBufferImpl::clearDepthStencil(const float depth, const uint8_t stencil)
    {
        dkCmdBufClearDepthStencil(m_Handle, true, depth, 0xFF, stencil);
    }

    void FCommandBufferImpl::clearColorTexture(ITexture* texture, const FColor& color, const FTextureSubresourceRange& subresourceRange)
    {

    }

    void FCommandBufferImpl::clearColorTexture(ITexture* texture, const FColor& color)
    {
    }

    void FCommandBufferImpl::bindVertexBuffer(const IBuffer* buffer, const int64_t offset)
    {
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        dkCmdBufBindVtxBuffer(m_Handle, 0, bufferImpl->getDeviceAddress() + offset, bufferImpl->getSize());
    }

    void FCommandBufferImpl::bindIndexBuffer(const IBuffer* buffer, const int64_t offset, const EIndexFormat format)
    {
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        dkCmdBufBindIdxBuffer(m_Handle, convert<DkIdxFormat>(format), bufferImpl->getDeviceAddress() + offset);
    }

    void FCommandBufferImpl::bindGraphicsPipeline(const IGraphicsPipeline* pipeline)
    {

    }

    void FCommandBufferImpl::pushConstants(const IShader* shader, FShaderStageFlags stageFlags, const void* data,
        uint64_t offset, uint64_t size)
    {
    }

    void FCommandBufferImpl::beginRenderPass(const FRenderPassDesc& renderPassDesc)
    {
    }

    void FCommandBufferImpl::endRenderPass()
    {
    }

    void FCommandBufferImpl::setViewport(const FViewport& viewport)
    {
        const DkViewport viewports[] { convert<DkViewport>(viewport) };
        dkCmdBufSetViewports(m_Handle, 0, viewports, 1);
    }

    void FCommandBufferImpl::setScissor(const FScissor& scissor)
    {
        const DkScissor scissors[] { convert<DkScissor>(scissor) };
        dkCmdBufSetScissors(m_Handle, 0, scissors, 1);
    }

    void FCommandBufferImpl::draw(const FDrawCommand& drawCmd)
    {
        dkCmdBufDraw(m_Handle, DkPrimitive_Triangles, drawCmd.vertexCount, drawCmd.instanceCount, drawCmd.firstVertex, drawCmd.firstInstance);
    }

    void FCommandBufferImpl::drawIndexed(const FDrawIndexedCommand& drawIndexedCmd)
    {
        dkCmdBufDrawIndexed(m_Handle, DkPrimitive_Triangles,
            drawIndexedCmd.indexCount,
            drawIndexedCmd.instanceCount,
            drawIndexedCmd.firstIndex,
            drawIndexedCmd.vertexOffset,
            drawIndexedCmd.firstInstance);
    }

    void FCommandBufferImpl::drawIndirect(const IBuffer* buffer, const uint64_t offset, const uint32_t drawCount)
    {
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        dkCmdBufDrawIndirect(m_Handle, DkPrimitive_Triangles, bufferImpl->getDeviceAddress() + offset);
        (void)drawCount;
    }

    void FCommandBufferImpl::drawIndexedIndirect(const IBuffer* buffer, const uint64_t offset, const uint32_t drawCount)
    {
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        dkCmdBufDrawIndexedIndirect(m_Handle, DkPrimitive_Triangles, bufferImpl->getDeviceAddress() + offset);
        (void)drawCount;
    }

    void FCommandBufferImpl::bindComputePipeline(const IComputePipeline* pipeline)
    {
    }

    void FCommandBufferImpl::bindMaterial(const FMaterial* material)
    {
    }

    void FCommandBufferImpl::bindBindingSet(const IBindingSet* bindingSet, const IShader* shader)
    {
    }

    void FCommandBufferImpl::drawStaticMesh(const FStaticMesh* staticMesh, const FMaterial* material,
        const FMatrix4f& transform, const FCamera& camera)
    {
    }

    void FCommandBufferImpl::drawStaticMesh(const FStaticMesh* staticMesh, const FMatrix4f& transform,
        const FCamera& camera)
    {
    }

    void FCommandBufferImpl::dispatch(const uint32_t groupCountX, const uint32_t groupCountY, const uint32_t groupCountZ)
    {
        dkCmdBufDispatchCompute(m_Handle, groupCountX, groupCountY, groupCountZ);
    }

    void FCommandBufferImpl::dispatchIndirect(IBuffer* buffer, const int64_t offset)
    {
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        dkCmdBufDispatchComputeIndirect(m_Handle, bufferImpl->getDeviceAddress() + offset);
    }

    void FCommandBufferImpl::copyBuffer(IBuffer* srcBuffer, IBuffer* dstBuffer, const int64_t srcOffset, const int64_t dstOffset, const uint64_t size)
    {
        const FBufferImpl* srcBufferImpl = static_cast<const FBufferImpl*>(srcBuffer);
        const FBufferImpl* dstBufferImpl = static_cast<const FBufferImpl*>(dstBuffer);
        dkCmdBufCopyBuffer(m_Handle, srcBufferImpl->getDeviceAddress() + srcOffset, dstBufferImpl->getDeviceAddress() + dstOffset, size);
    }

    void FCommandBufferImpl::copyBufferToTexture(IBuffer* buffer, int64_t offset, uint64_t size, ITexture* texture, uint32_t arrayIndex, uint32_t mipLevel)
    {
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);

        DkCopyBuf region;
        region.addr = bufferImpl->getDeviceAddress() + offset;
        region.imageHeight = 0;
        region.rowLength = 0;

       // dkCmdBufCopyBufferToImage(m_Handle, &region, )
    }

    void FCommandBufferImpl::textureBarrier(const FTextureBarrier& barrier)
    {

    }

    void FCommandBufferImpl::bufferBarrier(const FBufferBarrier& barrier)
    {
    }

    void FCommandBufferImpl::setName(const FStringView name)
    {
        ICommandBuffer::setName(name);
    }
}
