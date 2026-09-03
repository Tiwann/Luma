#include "Luma/D3D12/CommandBufferImpl.h"
#include "Luma/D3D12/GpuDeviceImpl.h"
#include <directx/d3d12.h>

#include "Luma/Containers/StringConversion.h"
#include "Luma/D3D12/Buffer.h"
#include "Luma/D3D12/Conversions.h"
#include "Luma/Rendering/RenderPassDesc.h"
#include "Luma/Rendering/Texture.h"

namespace Luma::D3D12
{
    EQueueType FCommandBufferImpl::getCommandBufferType()
    {
        return m_QueueType;
    }

    bool FCommandBufferImpl::initialize(const FCommandBufferDesc& cmdBufferDesc)
    {
        if (!cmdBufferDesc.device) return false;

        FGpuDeviceImpl* device = static_cast<FGpuDeviceImpl*>(cmdBufferDesc.device);
        ID3D12Device13* deviceHandle = device->getHandle();
        ID3D12CommandAllocator* commandAllocator = device->getCommandAllocator(cmdBufferDesc.queueType);

        D3D12_COMMAND_LIST_TYPE cmdListType = convert<D3D12_COMMAND_LIST_TYPE>(cmdBufferDesc.queueType);

        ID3D12CommandList* cmdBuffer = nullptr;
        if (DX_FAILED(deviceHandle->CreateCommandList(0, cmdListType, commandAllocator, nullptr, IID_PPV_ARGS(&cmdBuffer))))
            return false;

        if (m_Handle) m_Handle->Release();
        if (DX_FAILED(cmdBuffer->QueryInterface(IID_PPV_ARGS(&m_Handle))))
            return false;

        if (DX_FAILED(m_Handle->Close()))
            return false;

        m_Device = device;
        m_QueueType = cmdBufferDesc.queueType;
        return true;
    }

    void FCommandBufferImpl::destroy()
    {
        if (m_Handle) m_Handle->Release();
        m_Handle = nullptr;
        m_QueueType = EQueueType::None;
        m_Device = nullptr;
    }

    void FCommandBufferImpl::reset()
    {
        m_Handle->Reset(m_Device->getCommandAllocator(m_QueueType), nullptr);
    }

    bool FCommandBufferImpl::begin()
    {
        if (!m_Handle) return false;
        return true;
    }

    void FCommandBufferImpl::end()
    {
        m_Handle->Close();
    }

    void FCommandBufferImpl::beginDebugGroup(FStringView name, const FColor& color)
    {

    }

    void FCommandBufferImpl::endDebugGroup()
    {

    }

    void FCommandBufferImpl::setName(FStringView name)
    {
        if (name.isEmpty()) return;
        FWideString newName = stringConvert<wchar_t, char>(name);
        m_Handle->SetName(*newName);
    }

    void FCommandBufferImpl::clearColor(uint32_t attachmentIndex, const FColor& color)
    {

    }

    void FCommandBufferImpl::clearDepthStencil(float depth, uint8_t stencil)
    {
    }

    void FCommandBufferImpl::clearColorTexture(ITexture* texture, const FColor& color, const FTextureSubresourceRange& subresourceRange)
    {
    }

    void FCommandBufferImpl::clearColorTexture(ITexture* texture, const FColor& color)
    {

    }

    void FCommandBufferImpl::bindVertexBuffer(const IBuffer* buffer, int64_t offset)
    {
        const D3D12_VERTEX_BUFFER_VIEW view(buffer->getDeviceAddress() + offset, buffer->getSize() - offset, 0);
        m_Handle->IASetVertexBuffers(0, 1, &view);
    }

    void FCommandBufferImpl::bindIndexBuffer(const IBuffer* buffer, int64_t offset, EIndexFormat format)
    {
        const D3D12_INDEX_BUFFER_VIEW view(buffer->getDeviceAddress() + offset, buffer->getSize() - offset, convert<DXGI_FORMAT>(format));
        m_Handle->IASetIndexBuffer(&view);
    }

    void FCommandBufferImpl::pushConstants(const IShaderProgram* shader, FShaderStageFlags stageFlags, const void* data,
        uint64_t offset, uint64_t size)
    {
    }

    void FCommandBufferImpl::bindRenderPipeline(const IRenderPipeline* pipeline)
    {
    }

    void FCommandBufferImpl::beginRenderPass(const FRenderPassDesc& renderPassDesc)
    {
    }

    void FCommandBufferImpl::endRenderPass()
    {
    }

    void FCommandBufferImpl::setScissors(const TArray<FScissor>& scissors)
    {
        TArray<D3D12_RECT> scs = scissors.transform<D3D12_RECT>([](const FScissor& s)
        {
            return D3D12_RECT(s.x, s.y, s.width, s.height);
        });

        m_Handle->RSSetScissorRects(scs.count(), scs.data());
    }

    void FCommandBufferImpl::setScissor(const FScissor& scissor)
    {
        const D3D12_RECT scissorRect(scissor.x, scissor.y, scissor.width, scissor.height);
        m_Handle->RSSetScissorRects(1, &scissorRect);
    }

    void FCommandBufferImpl::setViewports(const TArray<FViewport>& viewports)
    {
        TArray<D3D12_VIEWPORT> vps = viewports.transform<D3D12_VIEWPORT>([](const FViewport& v)
        {
            return D3D12_VIEWPORT(v.x, v.y, v.width, v.height, v.minDepth, v.maxDepth);
        });

        m_Handle->RSSetViewports(vps.count(), vps.data());
    }

    void FCommandBufferImpl::setViewport(const FViewport& viewport)
    {
        const D3D12_VIEWPORT vp { viewport.x, viewport.y, viewport.width, viewport.height, viewport.minDepth, viewport.maxDepth };
        m_Handle->RSSetViewports(1, &vp);
    }

    void FCommandBufferImpl::draw(const FDrawCommand& drawCmd)
    {
        m_Handle->DrawInstanced(drawCmd.vertexCount,
            drawCmd.instanceCount,
            drawCmd.firstVertex,
            drawCmd.firstInstance);
    }

    void FCommandBufferImpl::drawIndexed(const FDrawIndexedCommand& drawIndexedCmd)
    {
        m_Handle->DrawIndexedInstanced(
            drawIndexedCmd.indexCount,
            drawIndexedCmd.instanceCount,
            drawIndexedCmd.firstIndex,
            drawIndexedCmd.vertexOffset,
            drawIndexedCmd.firstInstance);
    }

    void FCommandBufferImpl::drawIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount)
    {
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        m_Handle->ExecuteIndirect(m_Device->getDrawIndirectSignature(), drawCount, bufferImpl->getHandle(), offset, nullptr, 0);
    }

    void FCommandBufferImpl::drawIndexedIndirect(const IBuffer* buffer, uint64_t offset, uint32_t drawCount)
    {
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        m_Handle->ExecuteIndirect(m_Device->getDrawIndexedIndirectSignature(), drawCount, bufferImpl->getHandle(), offset, nullptr, 0);
    }

    void FCommandBufferImpl::bindMaterial(const FMaterial* material)
    {
    }

    void FCommandBufferImpl::bindBindingSet(const IBindingSet* bindingSet, const IShaderProgram* shader)
    {
    }

    void FCommandBufferImpl::textureBarrier(const FTextureBarrier& barrier)
    {
    }

    void FCommandBufferImpl::bufferBarrier(const FBufferBarrier& barrier)
    {
    }

    void FCommandBufferImpl::bindDescriptorBuffer(const IBuffer* buffer)
    {

    }

    void FCommandBufferImpl::bindComputePipeline(const IComputePipeline* pipeline)
    {

    }

    void FCommandBufferImpl::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
    {
        m_Handle->Dispatch(groupCountX, groupCountY, groupCountZ);
    }

    void FCommandBufferImpl::dispatchIndirect(IBuffer* buffer, int64_t offset)
    {
        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        m_Handle->ExecuteIndirect(m_Device->getDispatchIndirectSignature(), 1, bufferImpl->getHandle(), offset, nullptr, 0);
    }

    void FCommandBufferImpl::copyBuffer(IBuffer* srcBuffer, IBuffer* dstBuffer, int64_t srcOffset, int64_t dstOffset, uint64_t size)
    {
        const FBufferImpl* srcImpl = static_cast<const FBufferImpl*>(srcBuffer);
        const FBufferImpl* dstImpl = static_cast<const FBufferImpl*>(dstBuffer);
        m_Handle->CopyBufferRegion(dstImpl->getHandle(), dstOffset, srcImpl->getHandle(), srcOffset, size);
    }

    void FCommandBufferImpl::copyBufferToTexture(IBuffer* buffer, int64_t offset, uint64_t size, ITexture* texture, uint32_t arraySlice, uint32_t mipLevel)
    {

    }
}
