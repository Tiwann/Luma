#pragma once
#include "BufferBinding.h"
#include "TextureBinding.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Containers/ArrayView.h"
#include "Luma/Containers/StringView.h"

namespace Luma
{
    struct IShader;
    struct IBuffer;
    struct ITexture;
    struct ISampler;

    struct IBindingGroup : IRefCounted<IBindingGroup>
    {
        IBindingGroup() = default;

        explicit IBindingGroup(IShader* shader, uint32_t groupIndex);

        virtual void destroy() = 0;

        virtual void bindTextures(uint32_t bindingIndex, TArrayView<const ITexture*> textures, ETextureBindingType bindingType) = 0;
        void bindTextures(const FString& name, TArrayView<const ITexture*> textures, ETextureBindingType bindingType);
        void bindTexture(uint32_t bindingIndex, const ITexture* texture, ETextureBindingType bindingType);
        void bindTexture(const FString& name, const ITexture* texture, ETextureBindingType bindingType);

        virtual void bindBuffer(uint32_t bindingIndex, const IBuffer* buffer, int64_t offset, uint64_t size, EBufferBindingType bindingType) = 0;
        void bindBuffer(const FString& name, const IBuffer* buffer, int64_t offset, uint64_t size, EBufferBindingType bindingType);

        virtual void bindSampler(uint32_t bindingIndex, const ISampler* sampler) = 0;
        void bindSampler(const FString& name, const ISampler* sampler);


        virtual void bindTexturesWithSampler(uint32_t bindingIndex, TArrayView<const ITexture*> textures, const ISampler* sampler) = 0;
        void bindTexturesWithSampler(const FString& name, TArrayView<const ITexture*> textures, const ISampler* sampler);
        void bindTextureWithSampler(uint32_t bindingIndex, const ITexture* texture, const ISampler* sampler);
        void bindTextureWithSampler(const FString& name, const ITexture* texture, const ISampler* sampler);

        IShader* getShader() const { return m_Shader; }
        uint32_t getGroupIndex() const { return m_GroupIndex; }
    protected:
        IShader* m_Shader = nullptr;
        uint32_t m_GroupIndex = 0;
    };
}
