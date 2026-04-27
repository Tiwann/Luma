#include "ComputePipelineImpl.h"
#include "ShaderImpl.h"
#include "RenderDeviceImpl.h"
#include <Volk/volk.h>


namespace Luma::Vulkan
{
    bool FComputePipelineImpl::initialize(const FComputePipelineDesc& pipelineDesc)
    {
        if (!pipelineDesc.device) return false;
        if (!pipelineDesc.shader) return false;

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(pipelineDesc.device);
        FShaderImpl* shader = static_cast<FShaderImpl*>(pipelineDesc.shader);
        const auto& modules = shader->getShaderModules();
        if (modules.count() < 1) return false;
        const VkShaderModule module = modules.first();

        VkPipelineShaderStageCreateInfo stageCreateInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
        stageCreateInfo.module = module;
        stageCreateInfo.pName = "main";
        stageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;

        VkComputePipelineCreateInfo createInfo = { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO };
        createInfo.stage = stageCreateInfo;
        createInfo.layout = shader->getPipelineLayout();

        vkDestroyPipeline(device->getHandle(), m_Handle, nullptr);
        if (VK_FAILED(vkCreateComputePipelines(device->getHandle(), nullptr, 1, &createInfo, nullptr, &m_Handle)))
            return false;

        m_Device = device;
        return true;
    }

    void FComputePipelineImpl::destroy()
    {
        if (!m_Device) return;
        vkDestroyPipeline(m_Device->getHandle(), m_Handle, nullptr);
        m_Handle = nullptr;
    }
}
