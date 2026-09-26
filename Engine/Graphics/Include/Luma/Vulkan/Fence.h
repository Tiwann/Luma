#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Fence.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class Device;

    class LUMA_GRAPHICS_API Fence : public RHI::Fence
    {
    public:
        bool initialize(const RHI::FenceDesc& fenceDesc) override;
        void destroy() override;

        uint64_t getCompletedValue() const override;
        void signalOnCPU(uint64_t value) override;
        bool waitOnCPU(uint64_t value, uint64_t timeoutNs = RHI::FENCE_WAIT_INFINITE) override;

        void setName(StringView name) override;

        VkSemaphore getHandle() const { return m_Handle; }
    private:
        Device* m_Device = nullptr;
        VkSemaphore m_Handle = nullptr;
    };
}
