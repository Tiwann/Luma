#pragma once
#include "Luma/Rendering/Shader.h"
#include "BindingSetLayoutImpl.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    class FShaderImpl : public IShader
    {
    public:
        bool initialize(const FShaderDesc& shaderDesc) override;
        void destroy() override;
        IBindingSet* createBindingSet(uint32_t setIndex) override;

        const TArray<VkShaderModule>& getShaderModules() const { return m_Modules; }
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        TArray<FBindingSetLayoutImpl> m_SetLayouts;
        TArray<VkShaderModule> m_Modules;
        VkPipelineLayout m_PipelineLayout = nullptr;
    };
}
