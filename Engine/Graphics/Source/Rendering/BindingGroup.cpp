#include "Luma/Rendering/BindingGroup.h"
#include "Luma/Rendering/Shader.h"

namespace Luma
{
    IBindingGroup::IBindingGroup(IShader* shader, uint32_t groupIndex)
        : m_Shader(shader), m_GroupIndex(groupIndex)
    {

    }

    void IBindingGroup::bindTextures(const FString& name, TArrayView<const ITexture*> textures, ETextureBindingType bindingType)
    {
        bindTextures(m_Shader->getBindingFromName(name), textures, bindingType);
    }

    void IBindingGroup::bindTexture(uint32_t bindingIndex, const ITexture* texture, ETextureBindingType bindingType)
    {
        bindTextures(bindingIndex, {texture}, bindingType);
    }

    void IBindingGroup::bindTexture(const FString& name, const ITexture* texture, ETextureBindingType bindingType)
    {
        bindTextures(m_Shader->getBindingFromName(name), {texture}, bindingType);
    }

    void IBindingGroup::bindBuffer(const FString& name, const IBuffer* buffer, int64_t offset, uint64_t size, EBufferBindingType bindingType)
    {
        bindBuffer(m_Shader->getBindingFromName(name), buffer, offset, size, bindingType);
    }

    void IBindingGroup::bindSampler(const FString& name, const ISampler* sampler)
    {
        bindSampler(m_Shader->getBindingFromName(name), sampler);
    }

    void IBindingGroup::bindTexturesWithSampler(const FString& name, TArrayView<const ITexture*> textures, const ISampler* sampler)
    {
        bindTexturesWithSampler(m_Shader->getBindingFromName(name), textures, sampler);
    }

    void IBindingGroup::bindTextureWithSampler(uint32_t bindingIndex, const ITexture* texture, const ISampler* sampler)
    {
        bindTexturesWithSampler(bindingIndex, {texture}, sampler);
    }

    void IBindingGroup::bindTextureWithSampler(const FString& name, const ITexture* texture, const ISampler* sampler)
    {
        bindTexturesWithSampler(m_Shader->getBindingFromName(name), texture, sampler);
    }
}
