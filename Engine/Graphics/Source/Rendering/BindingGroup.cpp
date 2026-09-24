#include "Luma/Rendering/BindingGroup.h"
#include "Luma/Rendering/Shader.h"

namespace Luma::RHI
{
    BindingGroup::BindingGroup(Shader* shader, uint32_t groupIndex)
        : m_Shader(shader), m_GroupIndex(groupIndex)
    {

    }

    void BindingGroup::bindTextures(const String& name, ArrayView<const Texture*> textures, TextureBindingType bindingType)
    {
        bindTextures(m_Shader->getBindingFromName(name), textures, bindingType);
    }

    void BindingGroup::bindTexture(uint32_t bindingIndex, const Texture* texture, TextureBindingType bindingType)
    {
        bindTextures(bindingIndex, {texture}, bindingType);
    }

    void BindingGroup::bindTexture(const String& name, const Texture* texture, TextureBindingType bindingType)
    {
        bindTextures(m_Shader->getBindingFromName(name), {texture}, bindingType);
    }

    void BindingGroup::bindBuffer(const String& name, const Buffer* buffer, int64_t offset, uint64_t size, BufferBindingType bindingType)
    {
        bindBuffer(m_Shader->getBindingFromName(name), buffer, offset, size, bindingType);
    }

    void BindingGroup::bindSampler(const String& name, const Sampler* sampler)
    {
        bindSampler(m_Shader->getBindingFromName(name), sampler);
    }

    void BindingGroup::bindTexturesWithSampler(const String& name, ArrayView<const Texture*> textures, const Sampler* sampler)
    {
        bindTexturesWithSampler(m_Shader->getBindingFromName(name), textures, sampler);
    }

    void BindingGroup::bindTextureWithSampler(uint32_t bindingIndex, const Texture* texture, const Sampler* sampler)
    {
        bindTexturesWithSampler(bindingIndex, {texture}, sampler);
    }

    void BindingGroup::bindTextureWithSampler(const String& name, const Texture* texture, const Sampler* sampler)
    {
        bindTexturesWithSampler(m_Shader->getBindingFromName(name), texture, sampler);
    }
}
