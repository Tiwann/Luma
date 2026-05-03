#pragma once
#include "Luma/Rendering/ImmediateExecutor.h"
#include "FenceImpl.h"
#include "CommandBufferImpl.h"

namespace Luma::Vulkan
{
    class FQueueImpl;

    class FImmediateExecutorImpl : public IImmediateExecutor
    {
    public:
        bool initialize(const FImmediateExecutorDesc& executorDesc) override;
        void destroy() override;
        void execute(const std::function<void(const ICommandBuffer* cmdBuffer)>& function) override;
        bool isValid() const override;
    private:
        FQueueImpl* m_Queue = nullptr;
        FCommandBufferImpl m_CmdBuffer;
        FFenceImpl m_Fence;
        bool m_IsValid = false;
    };
}
