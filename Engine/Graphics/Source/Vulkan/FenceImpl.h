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
        void wait(uint64_t timeoutNs) override;
        void reset() override;

        VkFence getHandle() const { return m_Handle; }
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        VkFence m_Handle = nullptr;
    };
}