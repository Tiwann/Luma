#pragma once
#include "Luma/Rendering/BindingSetLayout.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class FBindingSetLayoutImpl : public IBindingSetLayout
    {
    public:
        ~FBindingSetLayoutImpl() override = default;
        bool initialize(const FBindingSetLayoutDesc& layoutDesc) override;
        void destroy() override;

        VkDescriptorSetLayout getHandle() const;

    private:
        VkDescriptorSetLayout m_Handle = nullptr;
        FRenderDeviceImpl* m_Device = nullptr;
    };
}
