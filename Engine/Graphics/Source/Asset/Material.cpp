#include "Luma/Asset/Material.h"

#include "Luma/Rendering/RenderPipeline.h"
#include "Luma/Rendering/Shader.h"

namespace Luma
{
    static constexpr uint32_t MATERIAL_BINDING_SET = 0;

    bool Material::initialize(const MaterialDesc& materialDesc)
    {
        if (!materialDesc.device) return false;
        if (!materialDesc.shader) return false;

        /*m_BindingSet = materialDesc.shader->createBindingSet(MATERIAL_BINDING_SET);*/
        //if (!m_BindingSet) return false;

        m_Shader = materialDesc.shader;
        m_Device = materialDesc.device;
        return true;
    }

    void Material::destroy()
    {
        m_Device = nullptr;
        m_Shader = nullptr;
        //m_BindingSet = nullptr;
    }

    void Material::setSampler(const FString& name, const Sampler* sampler)
    {

    }

    void Material::setTexture(const FString& name, const ITexture* texture, const BindingType bindingType)
    {

    }

    void Material::setCombinedTextureSampler(const FString& name, const Sampler* sampler, const ITexture* texture)
    {

    }

    void Material::setBuffer(const FString& name, const Buffer* buffer, const uint64_t offset, const uint64_t size)
    {

    }
}
