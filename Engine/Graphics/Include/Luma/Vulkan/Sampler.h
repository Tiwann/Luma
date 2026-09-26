#pragma once
#include "Luma/Graphics/Export.h"
#include "Luma/Rendering/Sampler.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class Device;

    class LUMA_GRAPHICS_API Sampler : public RHI::Sampler
    {
    public:
        bool initialize(const RHI::SamplerDesc& samplerDesc) override;
        void destroy() override;
        VkSampler getHandle() const { return m_Handle; }
        ResourceState getResourceState() const final;
        void setName(StringView name) override;
    private:
        Device* m_Device = nullptr;
        VkSampler m_Handle = nullptr;
    };
}
