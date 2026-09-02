#include "Luma/Vulkan/ShaderImpl.h"
#include "Luma/Vulkan/RenderDeviceImpl.h"
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
        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(shaderDesc.device);
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

    void FShaderImpl::destroy()
    {
        const VkDevice deviceHandle = m_Device->getHandle();
        vkDestroyShaderModule(deviceHandle, m_ShaderModule, nullptr);
    }

    IBindingSet* FShaderImpl::createBindingSet(uint32_t setIndex) const
    {
        return nullptr;
    }
}
