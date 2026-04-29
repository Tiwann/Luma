#include "ShaderImpl.h"
#include "RenderDeviceImpl.h"
#include "BindingSetImpl.h"
#include "Luma/Rendering/ShaderCompileRequest.h"
#include "Luma/Rendering/ShaderCompiler.h"
#include <Volk/volk.h>


namespace Luma::Vulkan
{
    bool FShaderImpl::initialize(const FShaderDesc& shaderDesc)
    {
        if (!shaderDesc.device) return false;
        if (shaderDesc.stageFlags == EShaderStageBits::None) return false;
        if (shaderDesc.moduleName.isEmpty()) return false;
        if (shaderDesc.filepath.isEmpty()) return false;

        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(shaderDesc.device);

        FShaderCompileRequest request;
        request.setLanguage(EShadingLanguage::Slang);
        request.setCompileTarget(EShaderCompileTarget::SPIRV);
        request.setModuleInfo(shaderDesc.moduleName, shaderDesc.filepath);

        if (shaderDesc.stageFlags & EShaderStageBits::Vertex)
            request.addEntryPoint("vert", EShaderStageBits::Vertex);

        if (shaderDesc.stageFlags & EShaderStageBits::Fragment)
            request.addEntryPoint("frag", EShaderStageBits::Fragment);

        if (shaderDesc.stageFlags & EShaderStageBits::Compute)
            request.addEntryPoint("compute", EShaderStageBits::Compute);

        if (shaderDesc.stageFlags & EShaderStageBits::Geometry)
            request.addEntryPoint("geom", EShaderStageBits::Geometry);

        FShaderCompiler compiler(device->getSlangSession());
        compiler.addCompileRequest(request);

        const TArray<FShaderCompileResult> results = compiler.compile();
        if (results.isEmpty()) return false;

        const auto& result = results.first();
        if (!result.succeeded) return false;

        for (const auto& compiledData : result.compiledData)
        {
            VkShaderModuleCreateInfo moduleCreateInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
            moduleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(compiledData.blob.data());
            moduleCreateInfo.codeSize = compiledData.blob.size();
            moduleCreateInfo.flags = 0;

            VkShaderModule module;
            if (VK_FAILED(vkCreateShaderModule(device->getHandle(), &moduleCreateInfo, nullptr, &module)))
                return false;
            m_Modules.push_back({module, compiledData.stage});
        }

        TArray<FShaderPushConstantRange> pushConstantsRanges;
        for (const auto& reflectionData : result.reflectionData)
        {
            for (auto& setLayoutDesc : reflectionData.setLayoutDescs)
            {
                FBindingSetLayoutDesc layoutDesc(setLayoutDesc);
                layoutDesc.device = device;

                FBindingSetLayoutImpl setLayout;
                if (!setLayout.initialize(layoutDesc))
                    return false;
                m_SetLayouts.push_back(setLayout);
            }

            pushConstantsRanges.addRange(reflectionData.pushConstantRanges);
        }

        const auto toVulkanSetLayout = [](const FBindingSetLayoutImpl& setLayout) { return setLayout.getHandle(); };
        std::vector<VkDescriptorSetLayout> vulkanSetLayouts(m_SetLayouts.size());
        std::ranges::transform(m_SetLayouts, std::back_inserter(vulkanSetLayouts), toVulkanSetLayout);

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
        pipelineLayoutCreateInfo.pSetLayouts = vulkanSetLayouts.data();
        pipelineLayoutCreateInfo.setLayoutCount = vulkanSetLayouts.size();
        if (VK_FAILED(vkCreatePipelineLayout(device->getHandle(), &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout)))
            return false;

        m_Device = device;
        m_StageFlags = shaderDesc.stageFlags;
        return true;
    }

    void FShaderImpl::destroy()
    {
        if (!m_Device) return;

        vkDestroyPipelineLayout(m_Device->getHandle(), m_PipelineLayout, nullptr);
        m_PipelineLayout = nullptr;

        for (auto& setLayout : m_SetLayouts)
            setLayout.destroy();
        m_SetLayouts.clear();

        for (const auto& module : m_Modules)
            vkDestroyShaderModule(m_Device->getHandle(), module.handle, nullptr);
        m_Modules.clear();
    }

    IBindingSet* FShaderImpl::createBindingSet(uint32_t setIndex) const
    {
        const auto it = std::ranges::find_if(m_SetLayouts, [&setIndex](const FBindingSetLayoutImpl& s) { return s.getSetIndex() == setIndex; });
        if (it == m_SetLayouts.end()) return nullptr;

        FBindingSetDesc bindingSetDesc;
        bindingSetDesc.device = m_Device;
        bindingSetDesc.layout = &*it;

        FBindingSetImpl* bindingSet = new FBindingSetImpl();
        if (!bindingSet->initialize(bindingSetDesc))
        {
            delete bindingSet;
            return nullptr;
        }
        return bindingSet;
    }
}
