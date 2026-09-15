#pragma once
#include "Luma/Rendering/Shader.h"
#include "Luma/Containers/HashMap.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FGPUDeviceImpl;

    class FShaderImpl : public IShader
    {
    public:
        ~FShaderImpl() override = default;
        bool initialize(const FShaderDesc& desc) override;
        void destroy() override;

        const THashMap<EShaderStage, VkShaderModule>& getShaderModules() const;
        VkDescriptorSetLayout getDescriptorSetLayout(uint32_t set) const;
        VkPipelineLayout getPipelineLayout() const;
    private:
        VkPipelineLayout m_PipelineLayout = nullptr;
        THashMap<EShaderStage, VkShaderModule> m_ShaderModules;
        THashMap<uint32_t, VkDescriptorSetLayout> m_DescriptorSetLayouts;
    };
}
