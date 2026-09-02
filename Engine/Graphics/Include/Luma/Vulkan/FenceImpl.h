#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Fence.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class LUMA_GRAPHICS_API FFenceImpl : public IFence
    {
    public:
        bool initialize(const FFenceDesc& fenceDesc) override;
        void destroy() override;

        uint64_t getCompletedValue() const override;
        void signalOnCPU(uint64_t value) override;
        bool waitOnCPU(uint64_t value, uint64_t timeoutNs = FENCE_WAIT_INFINITE) override;

        void setName(FStringView name) override;

        VkSemaphore getHandle() const { return m_Handle; }
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        VkSemaphore m_Handle = nullptr;
    };
}
