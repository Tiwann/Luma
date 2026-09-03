#pragma once
#include "Luma/Rendering/BindingSetLayout.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FGpuDeviceImpl;

    class FBindingSetLayoutImpl : public IBindingSetLayout
    {
    public:
        bool initialize(const FBindingSetLayoutDesc& layoutDesc) override;
        void destroy() override;

        VkDescriptorSetLayout getHandle() const;

    private:
        VkDescriptorSetLayout m_Handle = nullptr;
        FGpuDeviceImpl* m_Device = nullptr;
    };
}
