#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Queue.h"
#include "Luma/Containers/Array.h"
#include "VulkanFwd.h"
#include <cstdint>


namespace Luma::Vulkan
{
    class FSwapchainImpl;
    class FCommandBufferImpl;
    class FFenceImpl;
    class FSemaphoreImpl;
    class FRenderDeviceImpl;

    class LUMA_GRAPHICS_API FQueueImpl final : public IQueue
    {
    public:
        FQueueImpl(FRenderDeviceImpl* device) : m_Device(device) {}

        bool executeCommandBuffer(const ICommandBuffer* cmdBuffer, IFence* signalFence = nullptr, FPipelineStageFlags stageMask = EPipelineStageBits::None) override;
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

        void setName(FStringView name) override;
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        VkQueue m_Handle = nullptr;
        uint32_t m_Index = 0xFFFFFFFF;
        TArray<const FSemaphoreImpl*> m_WaitSemaphores;
        TArray<const FSemaphoreImpl*> m_SignalSemaphores;
    };
}
