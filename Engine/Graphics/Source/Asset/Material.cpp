#include "Luma/Asset/Material.h"
#include "Luma/Rendering/BindingSet.h"
#include "Luma/Rendering/GraphicsPipeline.h"
#include "Luma/Rendering/Shader.h"

namespace Luma
{
    static constexpr uint32_t MATERIAL_BINDING_SET = 0;

    bool FMaterial::initialize(const FMaterialDesc& materialDesc)
    {
        if (!materialDesc.device) return false;
        if (!materialDesc.shader) return false;

        m_BindingSet = materialDesc.shader->createBindingSet(MATERIAL_BINDING_SET);
        if (!m_BindingSet) return false;

        FGraphicsPipelineDesc opaquePipelineDesc;
        opaquePipelineDesc.device = materialDesc.device;
        opaquePipelineDesc.shaderProgram = materialDesc.shader;
        //opaquePipelineDesc.

        m_Pipelines[EMaterialType::Opaque] = materialDesc.device->createGraphicsPipeline(opaquePipelineDesc);


        m_Shader = materialDesc.shader;
        m_Device = materialDesc.device;
        return true;
    }

    void FMaterial::destroy()
    {
        m_Device = nullptr;
        m_Shader = nullptr;
        m_BindingSet = nullptr;
    }

    void FMaterial::setSampler(const FString& name, const ISampler* sampler)
    {
        if (m_BindingSet) m_BindingSet->bindSampler(name, sampler);
    }

    void FMaterial::setTexture(const FString& name, const ITexture* texture, const EBindingType bindingType)
    {
        if (m_BindingSet) m_BindingSet->bindTexture(name, texture, bindingType);
    }

    void FMaterial::setCombinedTextureSampler(const FString& name, const ISampler* sampler, const ITexture* texture)
    {
        if (!m_BindingSet) m_BindingSet->bindCombinedSamplerTexture(name, sampler, texture);
    }

    void FMaterial::setBuffer(const FString& name, const IBuffer* buffer, const uint64_t offset, const uint64_t size)
    {
        if (m_BindingSet) m_BindingSet->bindBuffer(name, buffer, offset, size);
    }
}
