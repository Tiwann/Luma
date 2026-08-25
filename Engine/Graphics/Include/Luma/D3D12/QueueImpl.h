#pragma once
#include "Luma/Rendering/Queue.h"
#include "D3D12Fwd.h"

namespace Luma::D3D12
{
    class FRenderDeviceImpl;

    class FQueueImpl final : public IQueue
    {
    public:
        bool initialize(FRenderDeviceImpl* device, EQueueType queueType);
        void destroy();

        bool executeCommandBuffer(const ICommandBuffer* cmdBuffer, IFence* signalFence, FPipelineStageFlags stageMask) override;
        bool present(ISwapchain* swapchain, ISemaphore* waitSemaphore, uint32_t imageIndex) override;
        void waitForSemaphore(const ISemaphore* semaphore) override;
        void signalSemaphore(const ISemaphore* semaphore) override;

        ID3D12CommandQueue* getHandle() const { return m_Handle; }
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        ID3D12CommandQueue* m_Handle = nullptr;
        TArray<const ISemaphore*> m_WaitSemaphores;
        TArray<const ISemaphore*> m_SignalSemaphores;
    };


}
