#pragma once
#include "Luma/Rendering/Shader.h"
#include "Luma/Rendering/ShaderStage.h"
#include "Luma/Containers/HashMap.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class Device;

    class Shader : public RHI::Shader
    {
    public:
        ~Shader() override = default;
        bool initialize(const RHI::ShaderDesc& desc) override;
        void destroy() override;

        const HashMap<ShaderStage, VkShaderModule>& getShaderModules() const;
        VkDescriptorSetLayout getDescriptorSetLayout(uint32_t set) const;
        VkPipelineLayout getPipelineLayout() const;
    private:
        VkPipelineLayout m_PipelineLayout = nullptr;
        HashMap<ShaderStage, VkShaderModule> m_ShaderModules;
        HashMap<uint32_t, VkDescriptorSetLayout> m_DescriptorSetLayouts;
    };
}
