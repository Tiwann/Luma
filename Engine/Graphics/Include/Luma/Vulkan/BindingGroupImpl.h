#pragma once
#include "Luma/Rendering/BindingGroup.h"
#include "VulkanFwd.h"
#include "Luma/Containers/HashMap.h"

namespace Luma::Vulkan
{
    class FShaderImpl;

    class FBindingGroupImpl : public IBindingGroup
    {
    public:
        FBindingGroupImpl() = default;
        explicit FBindingGroupImpl(FShaderImpl* shader, uint32_t groupIndex);

        void destroy() override;
        void bindTextures(uint32_t bindingIndex, TArrayView<const ITexture*> textures, ETextureBindingType bindingType) override;
        void bindBuffer(uint32_t bindingIndex, const IBuffer* buffer, int64_t offset, uint64_t size, EBufferBindingType bindingType) override;
        void bindSampler(uint32_t bindingIndex, const ISampler* sampler) override;
        void bindTexturesWithSampler(uint32_t bindingIndex, TArrayView<const ITexture*> textures, const ISampler* sampler) override;

        VkDescriptorSet getDescriptorSet() const { return m_DescriptorSet; }
    private:
        VkDescriptorSet m_DescriptorSet = nullptr;
    };
}
