#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Sampler.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class LUMA_GRAPHICS_API FSamplerImpl : public ISampler
    {
    public:
        bool initialize(const FSamplerDesc& samplerDesc) override;
        void destroy() override;
        VkSampler getHandle() const { return m_Handle; }
        EResourceState getResourceState() override;
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        VkSampler m_Handle = nullptr;
    };
}
