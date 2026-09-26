#pragma once
#include "VulkanFwd.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Rendering/BindingGroup.h"

namespace Luma::Vulkan
{
    class Shader;

    class BindingGroup : public RHI::BindingGroup
    {
    public:
        BindingGroup() = default;
        explicit BindingGroup(Shader* shader, uint32_t groupIndex);

        void destroy() override;
        void bindTextures(uint32_t bindingIndex, ArrayView<const RHI::Texture*> textures, TextureBindingType bindingType) override;
        void bindBuffer(uint32_t bindingIndex, const RHI::Buffer* buffer, int64_t offset, uint64_t size, BufferBindingType bindingType) override;
        void bindSampler(uint32_t bindingIndex, const RHI::Sampler* sampler) override;
        void bindTexturesWithSampler(uint32_t bindingIndex, ArrayView<const RHI::Texture*> textures, const RHI::Sampler* sampler) override;

        VkDescriptorSet getDescriptorSet() const { return m_DescriptorSet; }
    private:
        VkDescriptorSet m_DescriptorSet = nullptr;
    };
}
