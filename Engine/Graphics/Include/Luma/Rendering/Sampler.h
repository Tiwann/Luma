#pragma once
#include "CompareOperation.h"
#include "Filter.h"
#include "SamplerAddressMode.h"
#include "Resource.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Memory/RefCounted.h"


namespace Luma
{
    struct FSamplerDesc
    {
        struct IRenderDevice* device = nullptr;
        ESamplerAddressMode addressModeU = ESamplerAddressMode::Repeat;
        ESamplerAddressMode addressModeV = ESamplerAddressMode::Repeat;
        ESamplerAddressMode addressModeW = ESamplerAddressMode::Repeat;
        EFilter minFilter = EFilter::Nearest;
        EFilter magFilter = EFilter::Nearest;
        bool anisotropyEnable = false;
        bool compareEnable = false;
        ECompareOp compareOp = ECompareOp::Always;
        bool unnormalizedCoordinates = false;
        float minLod = 0.0f;
        float maxLod = 1.0f;
        EFilter mipmapFilter = EFilter::Nearest;

        FSamplerDesc& withDevice(IRenderDevice* device);
        FSamplerDesc& withAddressModeUVW(ESamplerAddressMode u, ESamplerAddressMode v, ESamplerAddressMode w);
        FSamplerDesc& withAddressMode(ESamplerAddressMode in);
        FSamplerDesc& withFilter(EFilter inMinFilter, EFilter inMagFilter);
        FSamplerDesc& withLODRange(float min, float max);

        bool operator==(const FSamplerDesc&) const = default;
    };

    struct FSamplerDescHasher
    {
        static void hashCombine(uint64_t& seed, const uint64_t value)
        {
            seed ^= value + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
        }

        uint64_t operator()(const FSamplerDesc& samplerDesc) const
        {
            uint64_t seed = 0;

            hashCombine(seed, reinterpret_cast<uint64_t>(samplerDesc.device));

            hashCombine(seed, static_cast<uint64_t>(samplerDesc.addressModeU));
            hashCombine(seed, static_cast<uint64_t>(samplerDesc.addressModeV));
            hashCombine(seed, static_cast<uint64_t>(samplerDesc.addressModeW));

            hashCombine(seed, static_cast<uint64_t>(samplerDesc.minFilter));
            hashCombine(seed, static_cast<uint64_t>(samplerDesc.magFilter));
            hashCombine(seed, static_cast<uint64_t>(samplerDesc.mipmapFilter));

            hashCombine(seed, static_cast<uint64_t>(samplerDesc.compareOp));

            hashCombine(seed, samplerDesc.anisotropyEnable);
            hashCombine(seed, samplerDesc.compareEnable);
            hashCombine(seed, samplerDesc.unnormalizedCoordinates);

            hashCombine(seed, *reinterpret_cast<const uint32_t*>(&samplerDesc.minLod));
            hashCombine(seed, *reinterpret_cast<const uint32_t*>(&samplerDesc.maxLod));
            return seed;
        }
    };


    struct ISampler : IResource, IRefCounted<ISampler>
    {
        ISampler() = default;
        ~ISampler() override = default;

        virtual bool initialize(const FSamplerDesc& samplerDesc) = 0;
        virtual void destroy() = 0;
        virtual void setName(FStringView name) {}
        EResourceType getResourceType() const final { return EResourceType::Sampler;  }

        ESamplerAddressMode getAddressModeU() const { return m_AddressModeU; }
        ESamplerAddressMode getAddressModeV() const { return m_AddressModeU; }
        ESamplerAddressMode getAddressModeW() const { return m_AddressModeU; }
        EFilter getMinFilter() const { return m_MinFilter; }
        EFilter getMagFilter() const { return m_MagFilter; }
        bool isAnisotropyEnabled() const { return m_AnisotropyEnable; }
        bool isCompareEnabled() const { return m_CompareEnable; }
        ECompareOp getCompareOp() const { return m_CompareOp; }
        bool doesAllowUnnormalizedCoordinates() const { return m_UnnormalizedCoordinates; }
        float getMinLod() const { return m_MinLod; }
        float getMaxLod() const { return m_MaxLod; }
        EFilter getMipmapFilter() const { return m_MipmapFilter; }
    protected:
        ESamplerAddressMode m_AddressModeU = ESamplerAddressMode::Repeat;
        ESamplerAddressMode m_AddressModeV = ESamplerAddressMode::Repeat;
        ESamplerAddressMode m_AddressModeW = ESamplerAddressMode::Repeat;
        EFilter m_MinFilter = EFilter::Nearest;
        EFilter m_MagFilter = EFilter::Nearest;
        bool m_AnisotropyEnable = false;
        bool m_CompareEnable = false;
        ECompareOp m_CompareOp = ECompareOp::Always;
        bool m_UnnormalizedCoordinates = false;
        float m_MinLod = 0.0f;
        float m_MaxLod = 1.0f;
        EFilter m_MipmapFilter = EFilter::Nearest;
    };
}
