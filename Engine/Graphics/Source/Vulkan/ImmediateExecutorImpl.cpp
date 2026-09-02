#include "Luma/Vulkan/ImmediateExecutorImpl.h"
#include "Luma/Vulkan/QueueImpl.h"

namespace Luma::Vulkan
{
    bool FImmediateExecutorImpl::initialize(const FImmediateExecutorDesc& executorDesc)
    {
        if (!executorDesc.device) return false;
        if (!executorDesc.queue) return false;

        FCommandBufferDesc cmdBufferDesc;
        cmdBufferDesc.device = executorDesc.device;
        cmdBufferDesc.queue = executorDesc.queue;

        if (!m_CmdBuffer.initialize(cmdBufferDesc)) return false;

        if (!m_Fence.initialize(FFenceDesc(executorDesc.device, 0))) return false;
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

        if (m_CmdBuffer.begin())
        {
            function(&m_CmdBuffer);
            m_CmdBuffer.end();

            FFenceSignal signal;
            signal.fence = &m_Fence;
            signal.value = ++m_FenceValue;

            FQueueExecuteInfo execInfo;
            execInfo.cmdBuffers = &m_CmdBuffer;
            execInfo.signals = signal;

            m_Queue->executeCommandBuffers(execInfo);
            m_Fence.waitOnCPU(m_FenceValue);
        }
    }

    bool FImmediateExecutorImpl::isValid() const
    {
        return m_IsValid;
    }
}
