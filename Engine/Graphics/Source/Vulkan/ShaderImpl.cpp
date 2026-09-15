#include "Luma/Vulkan/ShaderImpl.h"
#include "Luma/Vulkan/GPUDeviceImpl.h"
#include "Luma/Vulkan/Conversions.h"
#include "Luma/Utility/SpirvReflectCommon.h"
#include <volk.h>
#include <spirv_reflect.h>

namespace Luma::Vulkan
{
    /*bool FShaderImpl::initialize(const FShaderDesc& shaderDesc)
    {
        if (!shaderDesc.device) return false;

        const auto& shaderCode = shaderDesc.shaderCode;
        FGpuDeviceImpl* device = static_cast<FGpuDeviceImpl*>(shaderDesc.device);
        const VkDevice deviceHandle = device->getHandle();

        SpvReflectShaderModule moduleReflect;
        spvReflectCreateShaderModule2(SPV_REFLECT_MODULE_FLAG_NO_COPY, shaderCode.size(), shaderCode.data(), &moduleReflect);
        if (moduleReflect.entry_point_count <= 0) return false;

        if (moduleReflect.entry_point_count > 1)
        {
            std::cout << "Combined shader byte code with multiple entry points is prohibited!\n";
            return false;
        }

        m_EntryPoint = moduleReflect.entry_point_name;
        m_Stage = getShaderStage(moduleReflect.shader_stage);

        VkShaderModuleCreateInfo createInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderDesc.shaderCode.data());
        createInfo.codeSize = shaderDesc.shaderCode.size();
        if (vkCreateShaderModule(deviceHandle, &createInfo, nullptr, &m_ShaderModule) != VK_SUCCESS)
            return false;

        spvReflectDestroyShaderModule(&moduleReflect);
        m_Device = device;
        return true;
    }*/

    bool FShaderImpl::initialize(const FShaderDesc& desc)
    {
        if (!desc.device) return false;

        FGPUDeviceImpl* device = static_cast<FGPUDeviceImpl*>(desc.device);
        const VkDevice deviceHandle = device->getHandle();

        for (const auto& [stage, code] : desc.shaderCodes)
        {
            VkShaderModule module = nullptr;

            SpvReflectShaderModule moduleReflect;
            if (spvReflectCreateShaderModule2(SPV_REFLECT_MODULE_FLAG_NO_COPY, code.size(), code.data(), &moduleReflect) != SPV_REFLECT_RESULT_SUCCESS)
                return false;

            const TBufferView<SpvReflectDescriptorSet> descriptorSets{moduleReflect.descriptor_sets, moduleReflect.descriptor_set_count};
            for (const auto& set : descriptorSets)
            {
                const uint32_t setIndex = set.set;
                if (set.binding_count == 0) continue;

                const TBufferView<SpvReflectDescriptorBinding> descriptorBindings{*set.bindings, set.binding_count};
                for (const auto& binding : descriptorBindings)
                {
                    const uint32_t bindingIndex = binding.binding;

                    FShaderBinding& cachedBinding = m_Bindings[setIndex][bindingIndex];
                    cachedBinding.bindingIndex = bindingIndex;
                    cachedBinding.visibility |= stage;
                    cachedBinding.name = binding.name;
                    cachedBinding.bindingType = getBindingType(binding.descriptor_type);
                    cachedBinding.arrayCount = binding.count;

                    m_NameToBindingCache[binding.name] = bindingIndex;
                }
            }

            TBufferView<SpvReflectBlockVariable> pushConstantsBlocks{moduleReflect.push_constant_blocks, moduleReflect.push_constant_block_count};
            for (const auto& pushConstantsBlock : pushConstantsBlocks)
            {
                FShaderPushConstantVariable* variable = m_PushConstantsVars.single([&pushConstantsBlock](const FShaderPushConstantVariable& pc) {
                        return pc.name == pushConstantsBlock.name;
                });

                if (variable)
                {
                    variable->name = pushConstantsBlock.name;
                    variable->offset = pushConstantsBlock.offset;
                    variable->size = pushConstantsBlock.size;
                    variable->visibility |= stage;
                } else
                {
                    FShaderPushConstantVariable newVariable;
                    newVariable.name = pushConstantsBlock.name;
                    newVariable.offset = pushConstantsBlock.offset;
                    newVariable.size = pushConstantsBlock.size;
                    newVariable.visibility = stage;
                    m_PushConstantsVars.add(newVariable);
                }
            }

            VkShaderModuleCreateInfo createInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
            createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
            createInfo.codeSize = code.size();
            if (vkCreateShaderModule(deviceHandle, &createInfo, nullptr, &module) != VK_SUCCESS)
                return false;

            m_ShaderModules[stage] = module;
            m_Stages |= stage;
            spvReflectDestroyShaderModule(&moduleReflect);
        }

        TArray<VkDescriptorBindingFlags> bindingFlags;
        TArray<VkDescriptorSetLayoutBinding> vkBindings;
        TArray<VkDescriptorSetLayout> setLayouts;

        for (const auto& [setIndex, bindings] : m_Bindings)
        {
            for (const auto& [bindingIndex, binding] : bindings)
            {
                VkDescriptorSetLayoutBinding vkBinding = { };
                vkBinding.binding = binding.bindingIndex;
                if (binding.bindingType == EBindingType::SampledTexture && binding.arrayCount == 0)
                    vkBinding.descriptorCount = 1024;
                else
                    vkBinding.descriptorCount = binding.arrayCount;
                vkBinding.stageFlags = convert<VkShaderStageFlags>(binding.visibility);
                vkBinding.descriptorType = convert<VkDescriptorType>(binding.bindingType);
                vkBinding.pImmutableSamplers = nullptr;

                uint32_t bindingFlag = binding.arrayCount == 0 && binding.bindingType == EBindingType::SampledTexture
                                           ? VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
                                           VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT : 0;
                bindingFlags.add(bindingFlag);
                vkBindings.add(vkBinding);
            }

            VkDescriptorSetLayout descriptorSetLayout = nullptr;

            VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagCreateInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO};
            bindingFlagCreateInfo.bindingCount = bindings.count();
            bindingFlagCreateInfo.pBindingFlags = bindingFlags.data();

            VkDescriptorSetLayoutCreateInfo layoutCreateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
            layoutCreateInfo.pNext = &bindingFlagCreateInfo;
            layoutCreateInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
            layoutCreateInfo.bindingCount = m_Bindings[setIndex].count();
            layoutCreateInfo.pBindings = vkBindings.data();

            vkDestroyDescriptorSetLayout(deviceHandle, descriptorSetLayout, nullptr);
            if (VK_FAILED(vkCreateDescriptorSetLayout(deviceHandle, &layoutCreateInfo, nullptr, &descriptorSetLayout)))
                return false;

            m_DescriptorSetLayouts[setIndex] = descriptorSetLayout;
            setLayouts.add(descriptorSetLayout);
        }



        TArray<VkPushConstantRange> pcRanges = convert<TArray<VkPushConstantRange>>(m_PushConstantsVars);

        VkPipelineLayoutCreateInfo createInfo{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        createInfo.pPushConstantRanges = pcRanges.data();
        createInfo.pushConstantRangeCount = pcRanges.count();
        createInfo.pSetLayouts = setLayouts.data();
        createInfo.setLayoutCount = setLayouts.count();

        if (VK_FAILED(vkCreatePipelineLayout(deviceHandle, &createInfo, nullptr, &m_PipelineLayout)))
            return false;

        m_Device = device;
        return true;
    }

    void FShaderImpl::destroy()
    {
        const FGPUDeviceImpl* device = static_cast<FGPUDeviceImpl*>(m_Device);
        const VkDevice deviceHandle = device->getHandle();
        vkDestroyPipelineLayout(deviceHandle, m_PipelineLayout, nullptr);

        for (const auto [setIndex, setLayout] : m_DescriptorSetLayouts)
            vkDestroyDescriptorSetLayout(deviceHandle, setLayout, nullptr);

        for (const auto [shader, module] : m_ShaderModules)
            vkDestroyShaderModule(deviceHandle, module, nullptr);
    }

    const THashMap<EShaderStage, VkShaderModule>& FShaderImpl::getShaderModules() const
    {
        return m_ShaderModules;
    }

    VkDescriptorSetLayout FShaderImpl::getDescriptorSetLayout(uint32_t set) const
    {
        return m_DescriptorSetLayouts[set];
    }

    VkPipelineLayout FShaderImpl::getPipelineLayout() const
    {
        return m_PipelineLayout;
    }
}
