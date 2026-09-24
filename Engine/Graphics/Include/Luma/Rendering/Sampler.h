#pragma once
#include <cstdint>

#include "CompareOperation.h"
#include "Filter.h"
#include "Resource.h"
#include "SamplerAddressMode.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/RefCounted.h"

namespace Luma::RHI
{
    struct SamplerDesc
    {
        struct Device* device = nullptr;
        SamplerAddressMode addressModeU = SamplerAddressMode::Repeat;
        SamplerAddressMode addressModeV = SamplerAddressMode::Repeat;
        SamplerAddressMode addressModeW = SamplerAddressMode::Repeat;
        Filter minFilter = Filter::Nearest;
        Filter magFilter = Filter::Nearest;
        bool anisotropyEnable = false;
        bool compareEnable = false;
        CompareOp compareOp = CompareOp::Always;
        bool unnormalizedCoordinates = false;
        float minLod = 0.0f;
        float maxLod = 1.0f;
        Filter mipmapFilter = Filter::Nearest;

        SamplerDesc& withDevice(Device* device);
        SamplerDesc& withAddressModeUVW(SamplerAddressMode u, SamplerAddressMode v, SamplerAddressMode w);
        SamplerDesc& withAddressMode(SamplerAddressMode in);
        SamplerDesc& withFilter(Filter inMinFilter, Filter inMagFilter);
        SamplerDesc& withLODRange(float min, float max);

        bool operator==(const SamplerDesc&) const = default;
    };

    struct FSamplerDescHasher
    {
        static void hashCombine(std::uint64_t& seed, const uint64_t value)
        {
            seed ^= value + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
        }

        uint64_t operator()(const SamplerDesc& samplerDesc) const
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


    struct Sampler : Resource, RefCounted<Sampler>
    {
        Sampler() = default;
        ~Sampler() override = default;

        virtual bool initialize(const SamplerDesc& samplerDesc) = 0;
        virtual void destroy() = 0;
        virtual void setName(StringView name) {}
        ResourceType getResourceType() const final { return ResourceType::Sampler;  }

        SamplerAddressMode getAddressModeU() const { return m_AddressModeU; }
        SamplerAddressMode getAddressModeV() const { return m_AddressModeU; }
        SamplerAddressMode getAddressModeW() const { return m_AddressModeU; }
        Filter getMinFilter() const { return m_MinFilter; }
        Filter getMagFilter() const { return m_MagFilter; }
        bool isAnisotropyEnabled() const { return m_AnisotropyEnable; }
        bool isCompareEnabled() const { return m_CompareEnable; }
        CompareOp getCompareOp() const { return m_CompareOp; }
        bool doesAllowUnnormalizedCoordinates() const { return m_UnnormalizedCoordinates; }
        float getMinLod() const { return m_MinLod; }
        float getMaxLod() const { return m_MaxLod; }
        Filter getMipmapFilter() const { return m_MipmapFilter; }
    protected:
        SamplerAddressMode m_AddressModeU = SamplerAddressMode::Repeat;
        SamplerAddressMode m_AddressModeV = SamplerAddressMode::Repeat;
        SamplerAddressMode m_AddressModeW = SamplerAddressMode::Repeat;
        Filter m_MinFilter = Filter::Nearest;
        Filter m_MagFilter = Filter::Nearest;
        bool m_AnisotropyEnable = false;
        bool m_CompareEnable = false;
        CompareOp m_CompareOp = CompareOp::Always;
        bool m_UnnormalizedCoordinates = false;
        float m_MinLod = 0.0f;
        float m_MaxLod = 1.0f;
        Filter m_MipmapFilter = Filter::Nearest;
    };
}
