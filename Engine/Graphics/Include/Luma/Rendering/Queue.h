#pragma once
#include "QueueType.h"

namespace Luma
{
    struct IQueue
    {
        IQueue() = default;
        virtual ~IQueue() = default;

        EQueueType getQueueType() const { return m_QueueType; }
        void setQueueType(const EQueueType queueType) { m_QueueType = queueType; }
    protected:
        EQueueType m_QueueType = EQueueType::None;
    };
}
