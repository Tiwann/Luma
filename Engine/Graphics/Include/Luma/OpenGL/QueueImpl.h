#pragma once
#include "Rendering/Queue.h"
#include "Rendering/QueueType.h"

namespace Luma::OpenGL
{
    class CommandBuffer;

    class QueueImpl final : public Luma::Queue
    {
    public:
        explicit QueueImpl(const QueueType type) : Luma::Queue(), m_Type{type} {}

        QueueType GetQueueType() const { return m_Type; }
        void Submit(Luma::CommandBuffer* commandBuffer, Semaphore* waitSemaphore, Semaphore* signalSemaphore, Fence* fence, uint32_t waitStagesMask) const override;
        void Submit(const Array<Luma::CommandBuffer*>& commandBuffers, const Array<Semaphore*>& waitSemaphores, const Array<Semaphore*>& signalSemaphores, Fence* fence, uint32_t waitStagesMask) const override;
        bool Present(const Luma::Swapchain& swapchain, const Luma::Semaphore* waitSemaphore, uint32_t imageIndex) const override;

    private:
        QueueType m_Type = QueueType::None;
    };
}