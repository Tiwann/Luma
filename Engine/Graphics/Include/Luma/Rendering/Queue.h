#pragma once
#include "Fence.h"
#include "QueueType.h"
#include "Luma/Containers/ArrayView.h"
#include "Luma/Containers/StringView.h"

namespace Luma::RHI
{
    struct CommandBuffer;
    struct ISwapchain;
    struct Fence;

    struct QueueExecuteInfo
    {
        TArrayView<const CommandBuffer*> cmdBuffers;
        TArrayView<FenceWait> waits;
        TArrayView<FenceSignal> signals;
    };

    struct Queue
    {
        Queue() = default;
        virtual ~Queue() = default;
        virtual void waitIdle() = 0;
        virtual bool executeCommandBuffers(const QueueExecuteInfo& executeInfo) = 0;

        QueueType getQueueType() const { return m_QueueType; }
        void setQueueType(const QueueType queueType) { m_QueueType = queueType; }

        virtual void setName(FStringView name) { }
    protected:
        QueueType m_QueueType = QueueType::None;
    };
}
