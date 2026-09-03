#pragma once
#include "Luma/Rendering/ShaderProgram.h"
#include "BindingSetLayoutImpl.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    class FGpuDeviceImpl;

    class FShaderImpl : public IShaderProgram
    {
    public:
        ~FShaderImpl() override = default;
        //bool initialize(const FShaderDesc& shaderDesc) override;
        void destroy() override;

        IBindingSet* createBindingSet(uint32_t setIndex) const override;

        VkShaderModule getShaderModule() const { return m_ShaderModule; }
    private:
        FGpuDeviceImpl* m_Device = nullptr;
        VkShaderModule m_ShaderModule = nullptr;
    };
}