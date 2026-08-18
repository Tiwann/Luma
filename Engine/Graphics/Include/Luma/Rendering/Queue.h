#pragma once
#include "QueueType.h"
#include "Luma/Rendering/PipelineStage.h"
#include "Luma/Containers/StringView.h"
namespace Luma
{
    struct ICommandBuffer;
    struct ISwapchain;
    struct IFence;
    struct ISemaphore;

    struct IQueue
    {
        IQueue() = default;
        virtual ~IQueue() = default;
        virtual bool executeCommandBuffer(const ICommandBuffer* cmdBuffer, IFence* signalFence = nullptr, FPipelineStageFlags stageMask = EPipelineStageBits::None) = 0;
        virtual bool present(ISwapchain* swapchain, ISemaphore* waitSemaphore, uint32_t imageIndex) = 0;
        virtual void waitForSemaphore(const ISemaphore* semaphore) = 0;
        virtual void signalSemaphore(const ISemaphore* semaphore) = 0;

        EQueueType getQueueType() const { return m_QueueType; }
        void setQueueType(const EQueueType queueType) { m_QueueType = queueType; }

        virtual void setName(FStringView name) { }
    protected:
        EQueueType m_QueueType = EQueueType::None;
    };
}
