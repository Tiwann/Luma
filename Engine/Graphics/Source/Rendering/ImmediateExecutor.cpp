#include "Luma/Rendering/ImmediateExecutor.h"
#include "Luma/Rendering/Device.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/Fence.h"
#include "Luma/Rendering/Queue.h"

namespace Luma::RHI
{

    ImmediateExecutor::ImmediateExecutor(Device* device, Queue* queue)
    {
        m_CmdBuf = device->createCommandBuffer(queue);
        m_Fence = device->createFence(0);

        m_Device = device;
        m_Queue = queue;
        m_IsValid = true;
    }

    ImmediateExecutor::~ImmediateExecutor()
    {
        m_CmdBuf = nullptr;
        m_Fence = nullptr;
        m_Queue = nullptr;
        m_Device = nullptr;
    }

    void ImmediateExecutor::execute(const std::function<void(CommandBuffer* cmdBuffer)>& function)
    {
        if (m_CmdBuf->begin())
        {
            function(m_CmdBuf.get());
            m_CmdBuf->end();

            FenceSync signal;
            signal.fence = m_Fence;
            signal.value = ++m_FenceValue;
            signal.stages = PipelineStages::AllCommands;

            QueueExecuteInfo execInfo;
            execInfo.signals = signal;
            execInfo.cmdBuffers = m_CmdBuf.get();

            m_Queue->executeCommandBuffers(execInfo);
            m_Fence->waitOnCPU(m_FenceValue);
        }
    }

    bool ImmediateExecutor::isValid() const
    {
        return m_IsValid;
    }
}
