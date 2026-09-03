#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Queue.h"
#include "Luma/Containers/Array.h"
#include "DekoFwd.h"
#include <cstdint>


namespace Luma::Deko3d
{
    class FSwapchainImpl;
    class FCommandBufferImpl;
    class FFenceImpl;
    class FSemaphoreImpl;
    class FGpuDeviceImpl;

    class LUMA_GRAPHICS_API FQueueImpl final : public IQueue
    {
    public:
        FQueueImpl() = default;

        bool initialize(FGpuDeviceImpl* device, EQueueType queueType);
        void destroy();
        bool executeCommandBuffer(const ICommandBuffer* cmdBuffer, IFence* signalFence = nullptr, FPipelineStageFlags stageMask = EPipelineStageBits::None) override;
        bool present(ISwapchain* swapchain, ISemaphore* waitSemaphore, uint32_t imageIndex) override;
        void waitForSemaphore(const ISemaphore* semaphore) override;
        void signalSemaphore(const ISemaphore* semaphore) override;

        DkQueue getHandle() const;
        const DkQueue* getHandlePtr() const;
        void setName(FStringView name) override;

        void waitIdle();
    private:
        FGpuDeviceImpl* m_Device = nullptr;
        DkQueue m_Handle = nullptr;
        TArray<const FSemaphoreImpl*> m_WaitSemaphores;
        TArray<const FSemaphoreImpl*> m_SignalSemaphores;
    };
}
