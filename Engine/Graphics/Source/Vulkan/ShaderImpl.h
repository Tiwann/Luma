#pragma once
#include "Luma/Rendering/Shader.h"
#include "BindingSetLayoutImpl.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FRenderDeviceImpl;

    struct FShaderModule
    {
        VkShaderModule handle = nullptr;
        EShaderStageBits stage = EShaderStageBits::None;
    };

    class FShaderImpl : public IShader
    {
    public:
        ~FShaderImpl() override = default;
        bool initialize(const FShaderDesc& shaderDesc) override;
        void destroy() override;
        IBindingSet* createBindingSet(uint32_t setIndex) override;

        const TArray<FShaderModule>& getShaderModules() const { return m_Modules; }
        VkPipelineLayout getPipelineLayout() const { return m_PipelineLayout; }

    private:
        FRenderDeviceImpl* m_Device = nullptr;
        TArray<FBindingSetLayoutImpl> m_SetLayouts;
        TArray<FShaderModule> m_Modules;
        VkPipelineLayout m_PipelineLayout = nullptr;
    };
}
