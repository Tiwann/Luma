#pragma once
#include "BufferBinding.h"
#include "TextureBinding.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Containers/ArrayView.h"
#include "Luma/Containers/StringView.h"

namespace Luma::RHI
{
    struct Shader;
    struct Buffer;
    struct Texture;
    struct Sampler;

    struct BindingGroup : RefCounted<BindingGroup>
    {
        BindingGroup() = default;

        explicit BindingGroup(Shader* shader, uint32_t groupIndex);

        virtual void destroy() = 0;

        virtual void bindTextures(uint32_t bindingIndex, ArrayView<const Texture*> textures, TextureBindingType bindingType) = 0;
        void bindTextures(const String& name, ArrayView<const Texture*> textures, TextureBindingType bindingType);
        void bindTexture(uint32_t bindingIndex, const Texture* texture, TextureBindingType bindingType);
        void bindTexture(const String& name, const Texture* texture, TextureBindingType bindingType);

        virtual void bindBuffer(uint32_t bindingIndex, const Buffer* buffer, int64_t offset, uint64_t size, BufferBindingType bindingType) = 0;
        void bindBuffer(const String& name, const Buffer* buffer, int64_t offset, uint64_t size, BufferBindingType bindingType);

        virtual void bindSampler(uint32_t bindingIndex, const Sampler* sampler) = 0;
        void bindSampler(const String& name, const Sampler* sampler);


        virtual void bindTexturesWithSampler(uint32_t bindingIndex, ArrayView<const Texture*> textures, const Sampler* sampler) = 0;
        void bindTexturesWithSampler(const String& name, ArrayView<const Texture*> textures, const Sampler* sampler);
        void bindTextureWithSampler(uint32_t bindingIndex, const Texture* texture, const Sampler* sampler);
        void bindTextureWithSampler(const String& name, const Texture* texture, const Sampler* sampler);

        Shader* getShader() const { return m_Shader; }
        uint32_t getGroupIndex() const { return m_GroupIndex; }
    protected:
        Shader* m_Shader = nullptr;
        uint32_t m_GroupIndex = 0;
    };
}
