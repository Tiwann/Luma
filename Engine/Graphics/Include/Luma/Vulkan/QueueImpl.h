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
    class FRenderDeviceImpl;

    class LUMA_GRAPHICS_API FQueueImpl final : public IQueue
    {
    public:
        explicit FQueueImpl(FRenderDeviceImpl* device);

        void waitIdle() override;
        bool executeCommandBuffers(const FQueueExecuteInfo& executeInfo) override;

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

        VkCommandPool createCommandPool();
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        VkQueue m_Handle = nullptr;
        uint32_t m_Index = uint32_t(-1);
    };
}
