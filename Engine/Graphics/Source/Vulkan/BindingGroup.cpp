#include "Luma/Vulkan/BindingGroup.h"
#include "Luma/Vulkan/Device.h"
#include "Luma/Vulkan/Shader.h"
#include "Luma/Vulkan/Buffer.h"
#include "Luma/Vulkan/Sampler.h"
#include "Luma/Vulkan/Conversions.h"
#include <volk.h>


namespace Luma::Vulkan
{
    BindingGroup::BindingGroup(Shader* shader, uint32_t groupIndex)
        : RHI::BindingGroup(shader, groupIndex)
    {
        LUMA_ASSERT(shader, "Shader should be valid!");
        const Device* device = static_cast<Device*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();
        const VkDescriptorPool descriptorPool = device->getDescriptorPool();
        const VkDescriptorSetLayout setLayout = shader->getDescriptorSetLayout(groupIndex);

        VkDescriptorSetAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        allocateInfo.descriptorPool = descriptorPool;
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &setLayout;

        vkAllocateDescriptorSets(deviceHandle, &allocateInfo, &m_DescriptorSet);
    }

    void BindingGroup::destroy()
    {
        const Device* device = static_cast<Device*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();
        const VkDescriptorPool descriptorPool = device->getDescriptorPool();
        vkFreeDescriptorSets(deviceHandle, descriptorPool, 1, &m_DescriptorSet);
    }

    void BindingGroup::bindTextures(uint32_t bindingIndex, ArrayView<const RHI::Texture*> textures, TextureBindingType bindingType)
    {
        if (textures.isEmpty()) return;

        const Device* device = static_cast<Device*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();

        Array<VkDescriptorImageInfo> imageInfos;
        for (const RHI::Texture* texture : textures)
        {
            const Texture* textureImpl = static_cast<const Texture*>(texture);
            const TextureView* textureView = static_cast<const TextureView*>(textureImpl->getTextureView());

            VkDescriptorImageInfo imageInfo;
            imageInfo.imageLayout = convert<VkImageLayout>(texture->getResourceState());
            imageInfo.imageView = textureView->getHandle();
            imageInfos.add(imageInfo);
        }

        VkWriteDescriptorSet descriptorWrite{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        descriptorWrite.dstSet = m_DescriptorSet;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorCount = textures.count();
        descriptorWrite.descriptorType = convert<VkDescriptorType>(bindingType);
        descriptorWrite.dstBinding = bindingIndex;
        descriptorWrite.pImageInfo = imageInfos.data();

        vkUpdateDescriptorSets(deviceHandle, 1, &descriptorWrite, 0, nullptr);
    }

    void BindingGroup::bindBuffer(uint32_t bindingIndex, const RHI::Buffer* buffer, int64_t offset, uint64_t size, BufferBindingType bindingType)
    {
        const Device* device = static_cast<Device*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();

        const Buffer* bufferImpl = static_cast<const Buffer*>(buffer);
        VkDescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = bufferImpl->getHandle();
        bufferInfo.offset = offset;
        bufferInfo.range = size;

        VkWriteDescriptorSet descriptorWrite{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        descriptorWrite.dstSet = m_DescriptorSet;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.descriptorType = convert<VkDescriptorType>(bindingType);
        descriptorWrite.dstBinding = bindingIndex;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(deviceHandle, 1, &descriptorWrite, 0, nullptr);
    }

    void BindingGroup::bindSampler(uint32_t bindingIndex, const RHI::Sampler* sampler)
    {
        const Device* device = static_cast<Device*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();

        const Sampler* samplerImpl = static_cast<const Sampler*>(sampler);
        const VkSampler samplerHandle = samplerImpl->getHandle();

        VkDescriptorImageInfo imageInfo;
        imageInfo.sampler = samplerHandle;

        VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        write.descriptorCount = 1;
        write.dstBinding = bindingIndex;
        write.dstArrayElement = 0;
        write.pImageInfo = &imageInfo;
        write.dstSet = m_DescriptorSet;
        vkUpdateDescriptorSets(deviceHandle, 1, &write, 0, nullptr);
    }

    void BindingGroup::bindTexturesWithSampler(uint32_t bindingIndex, ArrayView<const RHI::Texture*> textures, const RHI::Sampler* sampler)
    {
        const Device* device = static_cast<Device*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();

        const Sampler* samplerImpl = static_cast<const Sampler*>(sampler);
        const VkSampler samplerHandle = samplerImpl->getHandle();

        Array<VkDescriptorImageInfo> imageInfos;
        for (const RHI::Texture* texture : textures)
        {
            const Texture* textureImpl = static_cast<const Texture*>(texture);
            const TextureView* textureView = static_cast<const TextureView*>(textureImpl->getTextureView());

            VkDescriptorImageInfo imageInfo;
            imageInfo.sampler = samplerHandle;
            imageInfo.imageLayout = convert<VkImageLayout>(texture->getResourceState());
            imageInfo.imageView = textureView->getHandle();
            imageInfos.add(imageInfo);
        }

        VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.descriptorCount = textures.count();
        write.dstBinding = bindingIndex;
        write.dstArrayElement = 0;
        write.pImageInfo = imageInfos.data();
        write.dstSet = m_DescriptorSet;
        vkUpdateDescriptorSets(deviceHandle, 1, &write, 0, nullptr);
    }
}
