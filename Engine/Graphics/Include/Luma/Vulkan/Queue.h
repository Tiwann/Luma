#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Queue.h"
#include "VulkanFwd.h"
#include <cstdint>

namespace Luma::Vulkan
{
    class Swapchain;
    class CommandBuffer;
    class Fence;
    class Device;

    class LUMA_GRAPHICS_API Queue final : public RHI::Queue
    {
    public:
        explicit Queue(Device* device);

        void waitIdle() override;
        bool executeCommandBuffers(const RHI::QueueExecuteInfo& executeInfo) override;

        VkQueue getHandle() const;
        VkQueue* getHandlePtr();
        const VkQueue* getHandlePtr() const;

        void setIndex(uint32_t index);
        uint32_t getIndex() const;
        const uint32_t* getIndexPtr() const;

        bool sameIndex(const Queue& other) const;
        bool sameHandle(const Queue& other) const;
        bool same(const Queue& other) const;

        void setName(StringView name) override;

        VkCommandPool createCommandPool();
    private:
        Device* m_Device = nullptr;
        VkQueue m_Handle = nullptr;
        uint32_t m_Index = uint32_t(-1);
    };
}
