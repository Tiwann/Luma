#pragma once
#include "BindingType.h"
#include "BindingSetLayout.h"
#include "Luma/Containers/Array.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Memory/RefCounted.h"
#include <cstdint>


namespace Luma
{
    struct IRenderDevice;
    struct IBuffer;
    struct ISampler;
    struct ITexture;
    struct IBindingSetLayout;

    struct FBindingSetDesc
    {
        IRenderDevice* device = nullptr;
        const IBindingSetLayout* layout = nullptr;
    };

    struct IBindingSet : IRefCounted<IBindingSet>
    {
        IBindingSet() {}
        ~IBindingSet() override = default;

        virtual bool initialize(const FBindingSetDesc& bindingSetDesc) = 0;
        virtual void destroy() = 0;

        virtual bool bindTexture(uint32_t binding, const ITexture* texture, EBindingType bindingType) = 0;
        virtual bool bindTextures(uint32_t binding, const TArray<const ITexture*>& textures, EBindingType bindingType) = 0;
        virtual bool bindSampler(uint32_t binding, const ISampler* sampler) = 0;
        virtual bool bindCombinedSamplerTexture(uint32_t binding, const ISampler* sampler, const ITexture* texture) = 0;
        virtual bool bindCombinedSamplerTextures(uint32_t binding, const ISampler* sampler, const TArray<const ITexture*>& textures) = 0;
        virtual bool bindBuffer(uint32_t binding, const IBuffer* buffer, uint64_t offset, uint64_t size) = 0;

        virtual bool bindCombinedSamplerTexture(const FString& name, const ISampler* sampler, const ITexture* texture) = 0;
        virtual bool bindTexture(const FString& name, const ITexture* texture, EBindingType bindingType) = 0;
        virtual bool bindTextures(const FString& name, const TArray<const ITexture*>& textures, EBindingType bindingType) = 0;
        virtual bool bindSampler(const FString& name, const ISampler* sampler) = 0;
        virtual bool bindBuffer(const FString& name, const IBuffer* buffer, uint64_t offset, uint64_t size) = 0;

        uint32_t getSetIndex() const { return m_BindingSetLayout->getSetIndex(); }
        const IBindingSetLayout* getBindingSetLayout() const { return m_BindingSetLayout; }
    protected:
        const IBindingSetLayout* m_BindingSetLayout = nullptr;
        THashMap<FString, uint32_t> m_NameToBinding;
    };
}
