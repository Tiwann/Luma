#include "BindingSetLayoutImpl.h"
#include "RenderDeviceImpl.h"
#include "Conversions.h"
#include "Luma/Containers/Array.h"
#include <Volk/volk.h>


namespace Luma::Vulkan
{
    static constexpr uint32_t BINDLESS_MAX_TEXTURES = 1024;

    bool FBindingSetLayoutImpl::initialize(const FBindingSetLayoutDesc& layoutDesc)
    {
        if (!layoutDesc.device) return false;
        if (layoutDesc.bindings.isEmpty()) return false;

        TArray<VkDescriptorSetLayoutBinding> bindings;
        TArray<VkDescriptorBindingFlags> bindingFlags;

        for (const auto& binding : layoutDesc.bindings)
        {
            VkDescriptorSetLayoutBinding vkBinding = { };
            vkBinding.binding = binding.bindingIndex;
            if (binding.bindingType == EBindingType::SampledTexture && binding.arrayCount == 0)
                vkBinding.descriptorCount = BINDLESS_MAX_TEXTURES;
            else
                vkBinding.descriptorCount = binding.arrayCount;
            vkBinding.stageFlags = convert<VkShaderStageFlags>(binding.stageFlags);
            vkBinding.descriptorType = convert<VkDescriptorType>(binding.bindingType);
            vkBinding.pImmutableSamplers = nullptr;

            uint32_t bindingFlag = binding.arrayCount == 0 && binding.bindingType == EBindingType::SampledTexture
                                       ? VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
                                       VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT : 0;
            bindingFlags.add(bindingFlag);
            bindings.add(vkBinding);
        }

        VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagCreateInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO};
        bindingFlagCreateInfo.bindingCount = layoutDesc.bindings.count();
        bindingFlagCreateInfo.pBindingFlags = bindingFlags.data();

        VkDescriptorSetLayoutCreateInfo layoutCreateInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
        layoutCreateInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
        layoutCreateInfo.pNext = &bindingFlagCreateInfo;
        layoutCreateInfo.bindingCount = bindings.count();
        layoutCreateInfo.pBindings = bindings.data();

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(layoutDesc.device);
        const VkDevice deviceHandle = device->getHandle();

        vkDestroyDescriptorSetLayout(deviceHandle, m_Handle, nullptr);
        if (vkCreateDescriptorSetLayout(deviceHandle, &layoutCreateInfo, nullptr, &m_Handle) != VK_SUCCESS)
            return false;

        m_Device = device;
        m_Bindings = layoutDesc.bindings;
        return true;
    }

    void FBindingSetLayoutImpl::destroy()
    {
        if (!m_Device) return;
        const VkDevice deviceHandle = m_Device->getHandle();
        if (m_Handle) vkDestroyDescriptorSetLayout(deviceHandle, m_Handle, nullptr);
        m_Handle = nullptr;
        m_Device = nullptr;
        m_Bindings.clear();
    }

    VkDescriptorSetLayout FBindingSetLayoutImpl::getHandle() const
    {
        return m_Handle;
    }
}
