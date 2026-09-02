#pragma once
#include "Fence.h"
#include "QueueType.h"
#include "Luma/Containers/ArrayView.h"
#include "Luma/Containers/StringView.h"
namespace Luma
{
    struct ICommandBuffer;
    struct ISwapchain;
    struct IFence;

    struct FQueueExecuteInfo
    {
        TArrayView<const ICommandBuffer*> cmdBuffers;
        TArrayView<FFenceWait> waits;
        TArrayView<FFenceSignal> signals;
    };

    struct IQueue
    {
        IQueue() = default;
        virtual ~IQueue() = default;
        virtual void waitIdle() = 0;
        virtual bool executeCommandBuffers(const FQueueExecuteInfo& executeInfo) = 0;

        EQueueType getQueueType() const { return m_QueueType; }
        void setQueueType(const EQueueType queueType) { m_QueueType = queueType; }

        virtual void setName(FStringView name) { }
    protected:
        EQueueType m_QueueType = EQueueType::None;
    };
}
