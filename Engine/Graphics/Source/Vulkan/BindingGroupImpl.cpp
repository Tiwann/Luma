#include "Luma/Vulkan/BindingGroupImpl.h"
#include "Luma/Vulkan/ShaderImpl.h"
#include "Luma/Vulkan/GPUDeviceImpl.h"
#include "Luma/Vulkan/BufferImpl.h"
#include "Luma/Vulkan/Conversions.h"
#include <volk.h>

#include "Luma/Vulkan/SamplerImpl.h"


namespace Luma::Vulkan
{
    FBindingGroupImpl::FBindingGroupImpl(FShaderImpl* shader, uint32_t groupIndex)
        : IBindingGroup(shader, groupIndex)
    {
        LUMA_ASSERT(shader, "Shader should be valid!");
        const FGPUDeviceImpl* device = static_cast<FGPUDeviceImpl*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();
        const VkDescriptorPool descriptorPool = device->getDescriptorPool();
        const VkDescriptorSetLayout setLayout = shader->getDescriptorSetLayout(groupIndex);

        VkDescriptorSetAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        allocateInfo.descriptorPool = descriptorPool;
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &setLayout;

        vkAllocateDescriptorSets(deviceHandle, &allocateInfo, &m_DescriptorSet);
    }

    void FBindingGroupImpl::destroy()
    {
        const FGPUDeviceImpl* device = static_cast<FGPUDeviceImpl*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();
        const VkDescriptorPool descriptorPool = device->getDescriptorPool();
        vkFreeDescriptorSets(deviceHandle, descriptorPool, 1, &m_DescriptorSet);
    }

    void FBindingGroupImpl::bindTextures(uint32_t bindingIndex, TArrayView<const ITexture*> textures, ETextureBindingType bindingType)
    {
        if (textures.isEmpty()) return;

        const FGPUDeviceImpl* device = static_cast<FGPUDeviceImpl*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();

        TArray<VkDescriptorImageInfo> imageInfos;
        for (const ITexture* texture : textures)
        {
            const FTextureImpl* textureImpl = static_cast<const FTextureImpl*>(texture);
            const FTextureViewImpl* textureView = static_cast<const FTextureViewImpl*>(textureImpl->getTextureView());

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

    void FBindingGroupImpl::bindBuffer(uint32_t bindingIndex, const IBuffer* buffer, int64_t offset, uint64_t size, EBufferBindingType bindingType)
    {
        const FGPUDeviceImpl* device = static_cast<FGPUDeviceImpl*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();

        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
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

    void FBindingGroupImpl::bindSampler(uint32_t bindingIndex, const ISampler* sampler)
    {
        const FGPUDeviceImpl* device = static_cast<FGPUDeviceImpl*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();

        const FSamplerImpl* samplerImpl = static_cast<const FSamplerImpl*>(sampler);
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

    void FBindingGroupImpl::bindTexturesWithSampler(uint32_t bindingIndex, TArrayView<const ITexture*> textures, const ISampler* sampler)
    {
        const FGPUDeviceImpl* device = static_cast<FGPUDeviceImpl*>(m_Shader->getDevice());
        const VkDevice deviceHandle = device->getHandle();

        const FSamplerImpl* samplerImpl = static_cast<const FSamplerImpl*>(sampler);
        const VkSampler samplerHandle = samplerImpl->getHandle();

        TArray<VkDescriptorImageInfo> imageInfos;
        for (const ITexture* texture : textures)
        {
            const FTextureImpl* textureImpl = static_cast<const FTextureImpl*>(texture);
            const FTextureViewImpl* textureView = static_cast<const FTextureViewImpl*>(textureImpl->getTextureView());

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
