#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Semaphore.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class LUMA_GRAPHICS_API FSemaphoreImpl final : public ISemaphore
    {
    public:
        bool initialize(const FSemaphoreDesc& semaphoreDesc) override;
        void destroy() override;

        uint64_t getValue() const override;
        void signal(uint64_t value) override;
        void wait(uint64_t value, uint64_t timeout) override;
        void setName(FStringView name) override;
        VkSemaphore getHandle() const { return m_Handle; }

    private:
        FRenderDeviceImpl* m_Device = nullptr;
        VkSemaphore m_Handle = nullptr;
    };
}
