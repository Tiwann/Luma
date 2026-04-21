#include "BindingSetImpl.h"
#include "BindingSetLayoutImpl.h"
#include "RenderDeviceImpl.h"
#include "BufferImpl.h"
#include "SamplerImpl.h"
#include "Conversions.h"
#include "Luma/Runtime/Assertion.h"
#include <Volk/volk.h>

namespace Luma::Vulkan
{
    bool FBindingSetImpl::initialize(const FBindingSetDesc& bindingSetDesc)
    {
        if (!bindingSetDesc.device) return false;
        if (!bindingSetDesc.layout) return false;


        FRenderDeviceImpl* device = static_cast<FRenderDeviceImpl*>(bindingSetDesc.device);
        const VkDescriptorPool descriptorPool = device->getDescriptorPool();

        const FBindingSetLayoutImpl* bindingSetLayout = static_cast<const FBindingSetLayoutImpl*>(bindingSetDesc.layout);
        const VkDescriptorSetLayout setLayoutHandle = bindingSetLayout->getHandle();

        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
        descriptorSetAllocateInfo.descriptorPool = descriptorPool;
        descriptorSetAllocateInfo.descriptorSetCount = 1;
        descriptorSetAllocateInfo.pSetLayouts = &setLayoutHandle;

        if (VK_FAILED(vkAllocateDescriptorSets(device->getHandle(), &descriptorSetAllocateInfo, &m_Handle)))
            return false;

        m_Device = device;
        m_BindingSetLayout = bindingSetDesc.layout;
        return true;
    }

    void FBindingSetImpl::destroy()
    {
        if (!m_Device) return;
        const VkDescriptorPool descriptorPool = m_Device->getDescriptorPool();
        assert(!VK_FAILED(vkFreeDescriptorSets(m_Device->getHandle(), descriptorPool, 1, &m_Handle)), "Failed to free descriptor set!");
    }

    bool FBindingSetImpl::bindTexture(const uint32_t binding, const ITexture* texture, const EBindingType bindingType)
    {
        if (!(bindingType == EBindingType::SampledTexture || bindingType == EBindingType::StorageTexture))
            return false;

        if (!texture) return false;
        if (!m_Device) return false;

        const FTextureImpl* vulkanTexture = static_cast<const FTextureImpl*>(texture);
        const FTextureViewImpl* textureView = static_cast<const FTextureViewImpl*>(vulkanTexture->getTextureView());
        VkDescriptorImageInfo imageInfo;
        imageInfo.imageLayout = convert<VkImageLayout>(texture->getState());
        imageInfo.imageView = textureView->getHandle();

        VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        write.descriptorType = convert<VkDescriptorType>(bindingType);
        write.descriptorCount = 1;
        write.dstBinding = binding;
        write.dstArrayElement = 0;
        write.pImageInfo = &imageInfo;
        write.dstSet = m_Handle;
        vkUpdateDescriptorSets(m_Device->getHandle(), 1, &write, 0, nullptr);
        return true;
    }

    bool FBindingSetImpl::bindTextures(const uint32_t binding, const TArray<const ITexture*>& textures, const EBindingType bindingType)
    {
        if (!m_Device) return false;
        if (textures.isEmpty()) return false;
        if (textures.any([](const ITexture* ptr) { return !ptr; })) return false;
        if (!(bindingType == EBindingType::SampledTexture || bindingType == EBindingType::StorageTexture))
            return false;

        TArray<VkDescriptorImageInfo> imageInfos;
        for (size_t textureIndex = 0; textureIndex < textures.count(); ++textureIndex)
        {
            const FTextureImpl* texture = static_cast<const FTextureImpl*>(textures[textureIndex]);
            const FTextureViewImpl* textureView = static_cast<const FTextureViewImpl*>(texture->getTextureView());
            VkDescriptorImageInfo imageInfo;
            imageInfo.imageLayout = convert<VkImageLayout>(texture->getState());
            imageInfo.imageView = textureView->getHandle();
            imageInfos.add(imageInfo);
        }

        VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        write.descriptorType = convert<VkDescriptorType>(bindingType);
        write.descriptorCount = textures.count();
        write.dstBinding = binding;
        write.dstArrayElement = 0;
        write.pImageInfo = imageInfos.data();
        write.dstSet = m_Handle;
        vkUpdateDescriptorSets(m_Device->getHandle(), 1, &write, 0, nullptr);
        return true;
    }

    bool FBindingSetImpl::bindSampler(const uint32_t binding, const ISampler* sampler)
    {
        if (!sampler) return false;
        if (!m_Device) return false;

        const VkSampler vulkanSampler = static_cast<const FSamplerImpl*>(sampler)->getHandle();
        VkDescriptorImageInfo imageInfo;
        imageInfo.sampler = vulkanSampler;

        VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        write.descriptorCount = 1;
        write.dstBinding = binding;
        write.dstArrayElement = 0;
        write.pImageInfo = &imageInfo;
        write.dstSet = m_Handle;
        vkUpdateDescriptorSets(m_Device->getHandle(), 1, &write, 0, nullptr);
        return true;
    }

    bool FBindingSetImpl::bindCombinedSamplerTexture(const uint32_t binding, const ISampler* sampler, const ITexture* texture)
    {
        if (!m_Device) return false;
        if (!sampler) return false;
        if (!texture) return false;

        const FTextureImpl* vulkanTexture = static_cast<const FTextureImpl*>(texture);
        const FTextureViewImpl* textureView = static_cast<const FTextureViewImpl*>(vulkanTexture->getTextureView());
        const FSamplerImpl* samplerImpl = static_cast<const FSamplerImpl*>(sampler);

        VkDescriptorImageInfo imageInfo;
        imageInfo.imageLayout = convert<VkImageLayout>(texture->getState());
        imageInfo.imageView = textureView->getHandle();
        imageInfo.sampler = samplerImpl->getHandle();

        VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.descriptorCount = 1;
        write.dstBinding = binding;
        write.dstArrayElement = 0;
        write.pImageInfo = &imageInfo;
        write.dstSet = m_Handle;
        vkUpdateDescriptorSets(m_Device->getHandle(), 1, &write, 0, nullptr);
        return true;
    }

    bool FBindingSetImpl::bindCombinedSamplerTextures(const uint32_t binding, const ISampler* sampler, const TArray<const ITexture*>& textures)
    {
        if (!m_Device) return false;
        if (!sampler) return false;
        if (textures.isEmpty()) return false;
        if (textures.any([](const ITexture* ptr) { return !ptr; })) return false;

        const FSamplerImpl* samplerImpl = static_cast<const FSamplerImpl*>(sampler);
        const VkSampler vulkanSampler = samplerImpl->getHandle();

        TArray<VkDescriptorImageInfo> imageInfos;
        for (size_t textureIndex = 0; textureIndex < textures.count(); ++textureIndex)
        {
            const FTextureImpl* texture = static_cast<const FTextureImpl*>(textures[textureIndex]);
            const FTextureViewImpl* textureView = static_cast<const FTextureViewImpl*>(texture->getTextureView());

            VkDescriptorImageInfo imageInfo;
            imageInfo.sampler = vulkanSampler;
            imageInfo.imageLayout = convert<VkImageLayout>(texture->getState());
            imageInfo.imageView = textureView->getHandle();
            imageInfos.add(imageInfo);
        }

        VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.descriptorCount = textures.count();
        write.dstBinding = binding;
        write.dstArrayElement = 0;
        write.pImageInfo = imageInfos.data();
        write.dstSet = m_Handle;
        vkUpdateDescriptorSets(m_Device->getHandle(), 1, &write, 0, nullptr);
        return true;
    }

    bool FBindingSetImpl::bindBuffer(const uint32_t binding, const IBuffer* buffer, const uint64_t offset, const uint64_t size)
    {
        if (!m_Device) return false;
        if (!buffer) return false;

        const FBufferImpl* bufferImpl = static_cast<const FBufferImpl*>(buffer);
        VkDescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = bufferImpl->getHandle();
        bufferInfo.offset = offset;
        bufferInfo.range = size;

        VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
        switch (buffer->getUsage())
        {
        case EBufferUsage::None:
        case EBufferUsage::VertexBuffer:
        case EBufferUsage::IndexBuffer:
        case EBufferUsage::UniformBuffer:    descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; break;
        case EBufferUsage::StorageBuffer:    descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;break;
        default: break;
        }

        if (descriptorType == VK_DESCRIPTOR_TYPE_MAX_ENUM)
            return false;


        VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        write.descriptorType = descriptorType;
        write.descriptorCount = 1;
        write.dstBinding = binding;
        write.dstArrayElement = 0;
        write.pBufferInfo = &bufferInfo;
        write.dstSet = m_Handle;
        vkUpdateDescriptorSets(m_Device->getHandle(), 1, &write, 0, nullptr);
        return true;
    }

    bool FBindingSetImpl::bindTexture(const char* name, const ITexture* texture)
    {
        return false;
    }

    bool FBindingSetImpl::bindTextures(const char* name, const TArray<const ITexture*>& textures)
    {
        return false;
    }

    bool FBindingSetImpl::bindSampler(const char* name, const ISampler* sampler)
    {
        return false;
    }

    bool FBindingSetImpl::bindBuffer(const char* name, const IBuffer* buffer, uint64_t offset, uint64_t size)
    {
        return false;
    }
}
