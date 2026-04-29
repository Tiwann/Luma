#pragma once
#include "Luma/Rendering/BindingSet.h"
#include "VulkanFwd.h"

namespace Luma::Vulkan
{
    struct FRenderDeviceImpl;

    class FBindingSetImpl : public IBindingSet
    {
    public:
        bool initialize(const FBindingSetDesc& bindingSetDesc) override;
        void destroy() override;
        bool bindTexture(uint32_t binding, const ITexture* texture, EBindingType bindingType) override;
        bool bindTextures(uint32_t binding, const TArray<const ITexture*>& textures, EBindingType bindingType) override;
        bool bindSampler(uint32_t binding, const ISampler* sampler) override;
        bool bindCombinedSamplerTexture(uint32_t binding, const ISampler* sampler, const ITexture* texture) override;
        bool bindCombinedSamplerTextures(uint32_t binding, const ISampler* sampler, const TArray<const ITexture*>& textures) override;
        bool bindBuffer(uint32_t binding, const IBuffer* buffer, uint64_t offset, uint64_t size) override;
        bool bindTexture(const FString& name, const ITexture* texture, EBindingType bindingType) override;
        bool bindTextures(const FString& name, const TArray<const ITexture*>& textures, EBindingType bindingType) override;
        bool bindSampler(const FString& name, const ISampler* sampler) override;
        bool bindBuffer(const FString& name, const IBuffer* buffer, uint64_t offset, uint64_t size) override;

        VkDescriptorSet getHandle() const { return m_Handle; }
    private:
        FRenderDeviceImpl* m_Device = nullptr;
        VkDescriptorSet m_Handle = nullptr;
    };
}