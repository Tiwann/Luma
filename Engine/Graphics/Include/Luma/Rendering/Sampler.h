#pragma once
#include "CompareOperation.h"
#include "Filter.h"
#include "SamplerAddressMode.h"
#include "Resource.h"
#include "Luma/Containers/StringView.h"
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
