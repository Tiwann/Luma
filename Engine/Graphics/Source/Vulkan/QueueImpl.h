#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Queue.h"
#include "VulkanFwd.h"
#include <cstdint>


namespace Luma::Vulkan
{
    class FSwapchainImpl;
    class FCommandBufferImpl;
    class FFenceImpl;

    class LUMA_GRAPHICS_API FQueueImpl final : public IQueue
    {
    public:
        void submit(const FCommandBufferImpl* cmdBuffer, VkSemaphore waitSemaphore, VkSemaphore signalSemaphore, FFenceImpl* fence = nullptr, uint32_t waitStagesMask = 0) const;
        bool present(const FSwapchainImpl* swapchain, VkSemaphore waitSemaphore, uint32_t imageIndex) const;

        VkQueue getHandle() const;
        VkQueue* getHandlePtr();
        const VkQueue* getHandlePtr() const;

        void setIndex(uint32_t index);
        uint32_t getIndex() const;
        const uint32_t* getIndexPtr() const;

        bool sameIndex(const FQueueImpl& other) const;
        bool sameHandle(const FQueueImpl& other) const;
        bool same(const FQueueImpl& other) const;
        bool operator==(const FQueueImpl& other) const;
    private:
        VkQueue m_Handle = nullptr;
        uint32_t m_Index = 0xFFFFFFFF;
    };
}
