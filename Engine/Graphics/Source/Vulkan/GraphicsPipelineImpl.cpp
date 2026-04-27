#include "GraphicsPipelineImpl.h"
#include "RenderDeviceImpl.h"
#include "ShaderImpl.h"
#include "Conversions.h"
#include <Volk/volk.h>


namespace Luma::Vulkan
{
    bool FGraphicsPipelineImpl::initialize(const FGraphicsPipelineDesc& pipelineDesc)
    {
        if (!pipelineDesc.device) return false;
        if (!pipelineDesc.shaderProgram) return false;

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(pipelineDesc.device);
        FShaderImpl* shaderProgram = static_cast<FShaderImpl*>(pipelineDesc.shaderProgram);
        
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
        inputAssemblyState.primitiveRestartEnable = pipelineDesc.inputAssembly.primitiveRestartEnable;
        inputAssemblyState.topology = convert<VkPrimitiveTopology>(pipelineDesc.inputAssembly.topology);

        const auto& inputLayout = pipelineDesc.inputLayout;

        TArray<VkVertexInputBindingDescription> bindingDescs;
        TArray<VkVertexInputAttributeDescription> attributeDescs;

        for (const auto& binding : inputLayout.getInputBindings())
        {
            VkVertexInputBindingDescription bindingDesc;
            bindingDesc.binding = binding.key;
            bindingDesc.inputRate = convert<VkVertexInputRate>(binding.value);
            bindingDesc.stride = inputLayout.getStride(binding.key);
            bindingDescs.add(bindingDesc);
        }

        for (const auto& attribute : inputLayout.getInputAttributes())
        {
            static uint32_t location = 0;
            VkVertexInputAttributeDescription attributeDesc;
            attributeDesc.binding = attribute.binding;
            attributeDesc.location = location;
            attributeDesc.format = convert<VkFormat>(attribute.type);
            attributeDesc.offset = inputLayout.getAttributeOffset(location);
            location++;
            attributeDescs.add(attributeDesc);
        }

        VkPipelineVertexInputStateCreateInfo vertexInputState { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
        vertexInputState.pVertexAttributeDescriptions = attributeDescs.data();
        vertexInputState.vertexAttributeDescriptionCount = attributeDescs.count();
        vertexInputState.pVertexBindingDescriptions = bindingDescs.data();
        vertexInputState.vertexBindingDescriptionCount = bindingDescs.count();

        VkPipelineRasterizationStateCreateInfo rasterizationState { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
        rasterizationState.cullMode = convert<VkCullModeFlags>(pipelineDesc.rasterization.cullMode);
        rasterizationState.frontFace = convert<VkFrontFace>(pipelineDesc.rasterization.frontFace);
        rasterizationState.polygonMode = convert<VkPolygonMode>(pipelineDesc.rasterization.polygonMode);
        rasterizationState.rasterizerDiscardEnable = pipelineDesc.rasterization.discardEnable;
        rasterizationState.depthClampEnable = pipelineDesc.rasterization.depthClampEnable;
        rasterizationState.depthBiasEnable = pipelineDesc.rasterization.depthBiasEnable;
        rasterizationState.depthBiasClamp = pipelineDesc.rasterization.depthBiasClamp;
        rasterizationState.depthBiasConstantFactor = pipelineDesc.rasterization.depthBiasConstantFactor;
        rasterizationState.depthBiasSlopeFactor = pipelineDesc.rasterization.depthBiasSlopeFactor;
        rasterizationState.lineWidth = pipelineDesc.rasterization.lineWidth;

        VkPipelineColorBlendAttachmentState colorBlendStates[8];
        for (uint32_t i = 0; i < 8; i++)
        {
            const auto& blendState = pipelineDesc.colorBlend[i];

            VkPipelineColorBlendAttachmentState state;
            state.blendEnable = blendState.colorBlendEnable;
            state.colorWriteMask = blendState.colorWriteMask;
            if (blendState.colorBlendEnable)
            {
                state.alphaBlendOp = convert<VkBlendOp>(blendState.blendFunction.alphaOp);
                state.colorBlendOp = convert<VkBlendOp>(blendState.blendFunction.colorOp);
                state.dstAlphaBlendFactor = convert<VkBlendFactor>(blendState.blendFunction.alphaDest);
                state.dstColorBlendFactor = convert<VkBlendFactor>(blendState.blendFunction.colorDest);
                state.srcAlphaBlendFactor = convert<VkBlendFactor>(blendState.blendFunction.alphaSource);
                state.srcColorBlendFactor = convert<VkBlendFactor>(blendState.blendFunction.colorSource);
            }
        }

        VkPipelineColorBlendStateCreateInfo colorBlendState { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
        colorBlendState.pAttachments = colorBlendStates;
        colorBlendState.attachmentCount = pipelineDesc.colorFormatCount;
        
        VkPipelineDepthStencilStateCreateInfo depthStencilState { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
        depthStencilState.depthTestEnable = pipelineDesc.depthStencil.depthTestEnable;
        depthStencilState.depthWriteEnable = pipelineDesc.depthStencil.depthWriteEnable;
        depthStencilState.stencilTestEnable = pipelineDesc.depthStencil.stencilTestEnable;
        depthStencilState.depthCompareOp = convert<VkCompareOp>(pipelineDesc.depthStencil.depthCompareOp);
        
        VkSampleMask SampleMask = 0xFFFFFFFF;
        VkPipelineMultisampleStateCreateInfo multisampleState { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
        multisampleState.rasterizationSamples = static_cast<VkSampleCountFlagBits>(pipelineDesc.multisample.sampleCount);
        multisampleState.alphaToCoverageEnable = pipelineDesc.multisample.alphaToCoverageEnable;
        multisampleState.alphaToOneEnable = pipelineDesc.multisample.alphaToOneEnable;
        multisampleState.pSampleMask = &SampleMask;
        multisampleState.sampleShadingEnable = false;
        // MultisampleState.minSampleShading = ...; if sampleShading is true

        VkViewport viewport;
        viewport.x = pipelineDesc.viewport.x;
        viewport.y = pipelineDesc.viewport.y + pipelineDesc.viewport.height;
        viewport.width = pipelineDesc.viewport.width;
        viewport.height = -pipelineDesc.viewport.height;
        viewport.minDepth = pipelineDesc.viewport.minDepth;
        viewport.maxDepth = pipelineDesc.viewport.maxDepth;

        VkRect2D scissor;
        scissor.offset.x = pipelineDesc.scissor.x;
        scissor.offset.y = pipelineDesc.scissor.y;
        scissor.extent.width = pipelineDesc.scissor.width;
        scissor.extent.height = pipelineDesc.scissor.height;
        
        VkPipelineViewportStateCreateInfo viewportState { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        TArray<VkFormat> colorFormats;
        for (uint32_t i = 0; i < pipelineDesc.colorFormatCount && i < 8; i++)
        {
            EFormat format = pipelineDesc.colorFormats[i];
            colorFormats.add(convert<VkFormat>(format));
        }

        VkFormat depthAttachmentFormat = convert<VkFormat>(pipelineDesc.depthFormat);
        VkPipelineRenderingCreateInfo renderingInfo { VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
        renderingInfo.viewMask = 0;
        renderingInfo.pColorAttachmentFormats = colorFormats.data();
        renderingInfo.colorAttachmentCount = colorFormats.count();
        renderingInfo.depthAttachmentFormat = depthAttachmentFormat;
        renderingInfo.stencilAttachmentFormat = depthAttachmentFormat;

        TArray<VkPipelineShaderStageCreateInfo> shaderStages;
        for (const auto& module : shaderProgram->getShaderModules())
        {
            VkPipelineShaderStageCreateInfo shaderStage { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
            shaderStage.module = module.handle;
            shaderStage.pName = "main";
            shaderStage.stage = convert<VkShaderStageFlagBits>(module.stage);
            shaderStages.add(shaderStage);
        }

        TArray dynamicStates { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        VkPipelineDynamicStateCreateInfo dynamicState { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
        dynamicState.dynamicStateCount = dynamicStates.count();
        dynamicState.pDynamicStates =  dynamicStates.data();
        
        VkGraphicsPipelineCreateInfo pipelineCreateInfo { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
        pipelineCreateInfo.pNext = &renderingInfo;
        pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineCreateInfo.pRasterizationState = &rasterizationState;
        pipelineCreateInfo.pViewportState = &viewportState;
        pipelineCreateInfo.pColorBlendState = &colorBlendState;
        pipelineCreateInfo.pVertexInputState = &vertexInputState;
        pipelineCreateInfo.pDepthStencilState = &depthStencilState;
        pipelineCreateInfo.pDynamicState = &dynamicState;
        pipelineCreateInfo.pMultisampleState = &multisampleState;
        pipelineCreateInfo.pStages = shaderStages.data();
        pipelineCreateInfo.stageCount = shaderStages.count();
        pipelineCreateInfo.layout = shaderProgram->getPipelineLayout();
        
        const VkDevice deviceHandle = device->getHandle();
        vkDestroyPipeline(deviceHandle, m_Handle, nullptr);
        if (VK_FAILED(vkCreateGraphicsPipelines(deviceHandle, nullptr, 1, &pipelineCreateInfo, nullptr, &m_Handle)))
            return false;
        m_Device = device;
        return true;
    }

    void FGraphicsPipelineImpl::destroy()
    {
        if (!m_Device) return;
        vkDestroyPipeline(m_Device->getHandle(), m_Handle, nullptr);
        m_Handle = nullptr;
    }
}
