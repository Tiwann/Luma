#include "Luma/D3D12/QueueImpl.h"
#include "Luma/D3D12/GpuDeviceImpl.h"

#include <directx/d3d12.h>
#include <dxgi1_6.h>

#include "Luma/D3D12/CommandBufferImpl.h"

namespace Luma::D3D12
{
    bool FQueueImpl::initialize(FGpuDeviceImpl* device, EQueueType queueType)
    {
        if (!device) return false;
        auto* deviceHandle = device->getHandle();
        if (!deviceHandle) return false;

        if (m_Handle) m_Handle->Release();
        switch (queueType)
        {
        case EQueueType::None: return false;
        case EQueueType::Render:
            {
                D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
                commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
                commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
                commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
                commandQueueDesc.NodeMask = 0;
                if (DX_FAILED(deviceHandle->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_Handle))))
                    return false;
                break;
            }

        case EQueueType::Compute:
            {
                D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
                commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
                commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
                commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
                commandQueueDesc.NodeMask = 0;
                if (DX_FAILED(deviceHandle->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_Handle))))
                    return false;
                break;
            }

        case EQueueType::Copy:
            {
                D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
                commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
                commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
                commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
                commandQueueDesc.NodeMask = 0;
                if (DX_FAILED(deviceHandle->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_Handle))))
                    return false;
                break;
            }

        default: return false;
        }

        m_Device = device;
        m_QueueType = queueType;
        return true;
    }

    void FQueueImpl::destroy()
    {
        if (m_Handle) m_Handle->Release();
    }

    bool FQueueImpl::executeCommandBuffer(const ICommandBuffer* cmdBuffer, IFence* signalFence, FPipelineStageFlags stageMask)
    {
        if (!cmdBuffer) return false;

        const FCommandBufferImpl* cmdBufferImpl = static_cast<const FCommandBufferImpl*>(cmdBuffer);
        ID3D12CommandList* cmdBufferHandles[] = { cmdBufferImpl->getHandle() };
        m_Handle->ExecuteCommandLists(1, cmdBufferHandles);

        /*if (signalFence)
        {
            Fence* dxFence = static_cast<Fence*>(fence);
            ID3D12Fence1* fenceHandle = dxFence->GetHandle();
            const uint64_t fenceValue = dxFence->GetValue();

            (void)m_Handle->Signal(fenceHandle, fenceValue);
        }*/

        m_WaitSemaphores.clear();
        m_SignalSemaphores.clear();
        return true;
    }

    bool FQueueImpl::present(ISwapchain* swapchain, ISemaphore* waitSemaphore, uint32_t imageIndex)
    {
        if (!swapchain) return false;

        FSwapchainImpl* swapchainImpl = static_cast<FSwapchainImpl*>(swapchain);
        IDXGISwapChain4* swapchainHandle = swapchainImpl->getHandle();
        if (DX_FAILED(swapchainHandle->Present(m_Device->hasVSync(), 0)))
            return false;
        return true;
    }

    void FQueueImpl::waitForSemaphore(const ISemaphore* semaphore)
    {
        m_WaitSemaphores.addUnique(semaphore);
    }

    void FQueueImpl::signalSemaphore(const ISemaphore* semaphore)
    {
        m_SignalSemaphores.addUnique(semaphore);
    }
}
