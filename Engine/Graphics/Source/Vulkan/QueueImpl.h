#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Queue.h"
#include "VulkanFwd.h"
#include <cstdint>

#include "Luma/Containers/Array.h"


namespace Luma::Vulkan
{
    class FSwapchainImpl;
    class FCommandBufferImpl;
    class FFenceImpl;
    class FSemaphoreImpl;

    class LUMA_GRAPHICS_API FQueueImpl final : public IQueue
    {
    public:
        bool executeCommandBuffer(const ICommandBuffer* cmdBuffer, IFence* signalFence = nullptr, FPipelineStageFlags stageMask = EPipelineStageBits::None) const override;
        bool present(ISwapchain* swapchain, ISemaphore* waitSemaphore, uint32_t imageIndex) override;
        void waitForSemaphore(const ISemaphore* semaphore) override;
        void signalSemaphore(const ISemaphore* semaphore) override;

        VkQueue getHandle() const;
        VkQueue* getHandlePtr();
        const VkQueue* getHandlePtr() const;

        void setIndex(uint32_t index);
        uint32_t getIndex() const;
        const uint32_t* getIndexPtr() const;

        bool sameIndex(const FQueueImpl& other) const;
        bool sameHandle(const FQueueImpl& other) const;
        bool same(const FQueueImpl& other) const;


    private:
        VkQueue m_Handle = nullptr;
        uint32_t m_Index = 0xFFFFFFFF;
        TArray<const FSemaphoreImpl*> m_WaitSemaphores;
        TArray<const FSemaphoreImpl*> m_SignalSemaphores;
    };
}
