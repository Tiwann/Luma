#include "Luma/Vulkan/ComputePipeline.h"
#include "Luma/Vulkan/Shader.h"
#include "Luma/Vulkan/Device.h"
#include <volk.h>


namespace Luma::Vulkan
{
    bool ComputePipeline::initialize(const RHI::ComputePipelineDesc& pipelineDesc)
    {
        if (!pipelineDesc.device) return false;
        if (!pipelineDesc.shaderProgram) return false;

        Device* device = static_cast<Device*>(pipelineDesc.device);
        Shader* shader = static_cast<Shader*>(pipelineDesc.shaderProgram);

        const auto& modules = shader->getShaderModules();

        VkPipelineShaderStageCreateInfo stageCreateInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
        stageCreateInfo.module = modules[ShaderStage::Compute];
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

    void ComputePipeline::destroy()
    {
        if (!m_Device) return;
        vkDestroyPipeline(m_Device->getHandle(), m_Handle, nullptr);
        m_Handle = nullptr;
    }
}
