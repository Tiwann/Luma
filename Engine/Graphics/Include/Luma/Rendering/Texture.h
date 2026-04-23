#pragma once
#include "Luma/Runtime/Format.h"
#include "Luma/Containers/StringView.h"
#include "Resource.h"
#include "TextureUsage.h"
#include "TextureDimension.h"
#include <cstdint>

namespace Luma
{
    struct IRenderDevice;
    struct ITextureView;
}

namespace Luma
{
    struct FTextureDesc
    {
        IRenderDevice* device = nullptr;
        FTextureUsageFlags usageFlags = ETextureUsageBits::None;
        EFormat format = EFormat::None;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 1;
        uint32_t mipCount = 1;
        uint32_t sampleCount = 1;
        uint32_t arrayCount = 1;

        static FTextureDesc Texture1D(const uint32_t width, const EFormat format, const uint32_t mipCount = 1, const uint32_t sampleCount = 1)
        {
            FTextureDesc desc;
            desc.usageFlags = ETextureUsageBits::Sampled;
            desc.format = format;
            desc.width = width;
            desc.height = 1;
            desc.depth = 1;
            desc.mipCount = mipCount;
            desc.sampleCount = sampleCount;
            desc.arrayCount = 1;
            return desc;
        }

        static FTextureDesc Texture2D(const uint32_t width, const uint32_t height, const EFormat format, const uint32_t mipCount = 1, const uint32_t sampleCount = 1)
        {
            FTextureDesc desc;
            desc.usageFlags = ETextureUsageBits::Sampled;
            desc.format = format;
            desc.width = width;
            desc.height = height;
            desc.depth = 1;
            desc.mipCount = mipCount;
            desc.sampleCount = sampleCount;
            desc.arrayCount = 1;
            return desc;
        }

        static FTextureDesc Texture3D(const uint32_t width, const uint32_t height, const uint32_t depth, const EFormat format, const uint32_t mipCount = 1, const uint32_t sampleCount = 1)
        {
            FTextureDesc desc;
            desc.usageFlags = ETextureUsageBits::Sampled;
            desc.format = format;
            desc.width = width;
            desc.height = height;
            desc.depth = depth;
            desc.mipCount = mipCount;
            desc.sampleCount = sampleCount;
            desc.arrayCount = 1;
            return desc;
        }
    };

    struct ITexture : IResource
    {
        ITexture() = default;
        ~ITexture() override = default;

        virtual bool initialize(const FTextureDesc& createInfo) = 0;
        virtual void destroy() = 0;
        virtual bool isValid() = 0;
        virtual void setName(FStringView name) {}
        EResourceType getResourceType() final { return EResourceType::Texture; }
        EFormat getFormat() const { return m_Format; }
        uint32_t getWidth() const { return m_Width; }
        uint32_t getHeight() const { return m_Height; }
        uint32_t getDepth() const { return m_Depth; }
        uint32_t getMipCount() const { return m_Mips; }
        uint32_t getSampleCount() const { return m_SampleCount; }
        EResourceState getState() const { return m_State; }
        uint32_t getArrayCount() const { return m_ArrayCount; }
        EResourceState getResourceState() override { return m_State; }
        void setResourceState(const EResourceState state) { m_State = state; }
        FTextureUsageFlags getUsageFlags() const { return m_UsageFlags; }
        ETextureDimension getDimension() const { return m_Dimension; }

        virtual const ITextureView* getTextureView() const = 0;
    protected:
        EFormat m_Format = EFormat::None;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_Depth = 0;
        uint32_t m_Mips = 0;
        uint32_t m_SampleCount = 0;
        uint32_t m_ArrayCount = 0;
        EResourceState m_State = EResourceState::Undefined;
        FTextureUsageFlags m_UsageFlags = ETextureUsageBits::None;
        ETextureDimension m_Dimension = ETextureDimension::None;
    };
}
