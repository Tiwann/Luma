#include "SamplerImpl.h"
#include "RenderDeviceImpl.h"
#include "Conversions.h"
#include "VulkanUtils.h"
#include <Volk/volk.h>
#include <iostream>


namespace Luma::Vulkan
{
    bool FSamplerImpl::initialize(const FSamplerDesc& samplerDesc)
    {
        if (!samplerDesc.device)
        {
            std::cerr << "[RENDER DEVICE] Failed to create sampler: device is null!\n";
            return false;
        }

        VkSamplerCreateInfo samplerci = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
        samplerci.addressModeU = convert<VkSamplerAddressMode>(samplerDesc.addressModeU);
        samplerci.addressModeV = convert<VkSamplerAddressMode>(samplerDesc.addressModeV);
        samplerci.addressModeW = convert<VkSamplerAddressMode>(samplerDesc.addressModeW);
        samplerci.anisotropyEnable = samplerDesc.anisotropyEnable;
        samplerci.compareEnable = samplerDesc.compareEnable;
        samplerci.compareOp = convert<VkCompareOp>(samplerDesc.compareOp);
        samplerci.minFilter = convert<VkFilter>(samplerDesc.minFilter);
        samplerci.magFilter = convert<VkFilter>(samplerDesc.magFilter);
        samplerci.minLod = samplerDesc.minLod;
        samplerci.maxLod = samplerDesc.maxLod;
        samplerci.unnormalizedCoordinates = samplerDesc.unnormalizedCoordinates;
        samplerci.mipLodBias = 0.0f;
        samplerci.mipmapMode = convert<VkSamplerMipmapMode>(samplerDesc.mipmapFilter);

        FRenderDeviceImpl* renderDevice = static_cast<FRenderDeviceImpl*>(samplerDesc.device);
        const VkDevice deviceHandle = renderDevice->getHandle();

        vkDestroySampler(deviceHandle, m_Handle, nullptr);
        if (VK_FAILED(vkCreateSampler(deviceHandle, &samplerci, nullptr, &m_Handle)))
        {
            std::cerr << "[RENDER DEVICE] Failed to create sampler: vkCreateSampler failure!\n";
            return false;
        }

        m_Device = renderDevice;
        m_AddressModeU = samplerDesc.addressModeU;
        m_AddressModeV = samplerDesc.addressModeV;
        m_AddressModeW = samplerDesc.addressModeW;
        m_MinFilter = samplerDesc.minFilter;
        m_MagFilter = samplerDesc.magFilter;
        m_AnisotropyEnable = samplerDesc.anisotropyEnable;
        m_CompareEnable = samplerDesc.compareEnable;
        m_CompareOp = samplerDesc.compareOp;
        m_UnnormalizedCoordinates = samplerDesc.unnormalizedCoordinates;
        m_MinLod = samplerDesc.minLod;
        m_MaxLod = samplerDesc.maxLod;
        m_MipmapFilter = samplerDesc.mipmapFilter;
        return true;
    }

    void FSamplerImpl::destroy()
    {
        const VkDevice deviceHandle = m_Device->getHandle();
        vkDestroySampler(deviceHandle, m_Handle, nullptr);
        m_Handle = nullptr;
    }

    EResourceState FSamplerImpl::getResourceState()
    {
        return EResourceState::General;
    }

    void FSamplerImpl::setName(FStringView name)
    {
        setVulkanObjectDebugName(m_Device, VK_OBJECT_TYPE_SAMPLER, m_Handle, name);
    }
}
