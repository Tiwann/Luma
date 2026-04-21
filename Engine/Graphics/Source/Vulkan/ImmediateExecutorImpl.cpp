#include "ImmediateExecutorImpl.h"
#include "QueueImpl.h"

namespace Luma::Vulkan
{
    bool FImmediateExecutorImpl::initialize(const FImmediateExecutorDesc& executorDesc)
    {
        if (!executorDesc.device) return false;
        if (!executorDesc.queue) return false;

        FCommandBufferDesc cmdBufferDesc;
        cmdBufferDesc.device = executorDesc.device;
        cmdBufferDesc.queueType = executorDesc.queue->getQueueType();
        if (!m_CmdBuffer.initialize(cmdBufferDesc)) return false;

        if (!m_Fence.initialize(FFenceDesc(executorDesc.device))) return false;
        m_Queue = static_cast<FQueueImpl*>(executorDesc.queue);
        m_IsValid = true;
        return true;
    }

    void FImmediateExecutorImpl::destroy()
    {
        m_CmdBuffer.destroy();
        m_Fence.destroy();
        m_Queue = nullptr;
        m_IsValid = false;
    }

    void FImmediateExecutorImpl::execute(const std::function<void(const ICommandBuffer* cmdBuffer)>& function)
    {
        if (!m_IsValid) return;
        if (!m_Queue) return;
        if (!function) return;

        m_CmdBuffer.reset();
        m_Fence.reset();

        if (m_CmdBuffer.begin())
        {
            function(&m_CmdBuffer);
            m_CmdBuffer.end();
            m_Queue->executeCommandBuffer(&m_CmdBuffer, &m_Fence, EPipelineStageBits::ColorTargetOutput);
            m_Fence.wait(FENCE_WAIT_INFINITE);
        }
    }

    bool FImmediateExecutorImpl::isValid() const
    {
        return m_IsValid;
    }
}
