#pragma once
#include "Resource.h"
#include "ResourceState.h"
#include "ResourceType.h"
#include "TextureDimension.h"
#include "TextureUsage.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Math/Functions.h"
#include "Luma/Math/Rect3.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Runtime/Format.h"

namespace Luma::RHI
{
    struct Device;

    struct TextureDesc
    {
        Device* device = nullptr;
        TextureUsageFlags usageFlags = TextureUsage::None;
        Format format = Format::None;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 1;
        uint32_t mipCount = 1;
        uint32_t sampleCount = 1;
        uint32_t arrayCount = 1;

        static TextureDesc texture1D(const uint32_t width, const Format format, const uint32_t mipCount = 1, const uint32_t sampleCount = 1)
        {
            TextureDesc desc;
            desc.usageFlags = TextureUsage::Sampled | TextureUsage::Color;
            desc.format = format;
            desc.width = width;
            desc.height = 1;
            desc.depth = 1;
            desc.mipCount = mipCount;
            desc.sampleCount = sampleCount;
            desc.arrayCount = 1;
            return desc;
        }

        static TextureDesc texture2D(const uint32_t width, const uint32_t height, const Format format, const uint32_t mipCount = 1, const uint32_t sampleCount = 1)
        {
            TextureDesc desc;
            desc.usageFlags = TextureUsage::Sampled | TextureUsage::Color;
            desc.format = format;
            desc.width = width;
            desc.height = height;
            desc.depth = 1;
            desc.mipCount = mipCount;
            desc.sampleCount = sampleCount;
            desc.arrayCount = 1;
            return desc;
        }

        static TextureDesc texture3D(const uint32_t width, const uint32_t height, const uint32_t depth, const Format format, const uint32_t mipCount = 1, const uint32_t sampleCount = 1)
        {
            TextureDesc desc;
            desc.usageFlags = TextureUsage::Sampled | TextureUsage::Color;
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

    struct TextureView;

    struct Texture : Resource, RefCounted<Texture>
    {
        Texture() = default;
        ~Texture() override = default;

        virtual bool initialize(const TextureDesc& createInfo) = 0;
        virtual bool resize(uint32_t width, uint32_t height = 1, uint32_t depth = 1) = 0;
        virtual void destroy() = 0;
        virtual bool isValid() = 0;
        virtual void setName(StringView name) {}
        ResourceType getResourceType() const final { return ResourceType::Texture; }
        Format getFormat() const { return m_Format; }
        uint32_t getWidth() const { return m_Width; }
        uint32_t getHeight() const { return m_Height; }
        uint32_t getDepth() const { return m_Depth; }
        uint32_t getMipCount() const { return m_Mips; }
        uint32_t getSampleCount() const { return m_SampleCount; }
        uint32_t getArrayCount() const { return m_ArrayCount; }
        ResourceState getResourceState() const final { return m_State; }
        void setResourceState(const ResourceState state) { m_State = state; }
        TextureUsageFlags getUsageFlags() const { return m_UsageFlags; }
        TextureDimension getDimension() const { return m_Dimension; }

        uint32_t getMipWidth(uint32_t mipLevel) const { return max(1u, m_Width >> mipLevel); }
        uint32_t getMipHeight(uint32_t mipLevel) const { return max(1u, m_Height >> mipLevel); }
        uint32_t getMipDepth(uint32_t mipLevel) const { return max(1u, m_Depth >> mipLevel); }

        FVector3u getSize() const { return FVector3u(m_Width, m_Height, m_Depth); }
        FRect3u getBounds() const { return FRect3u(0, 0, 0, m_Width, m_Height, m_Depth); }

        virtual const TextureView* getTextureView() const = 0;
    protected:
        Format m_Format = Format::None;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_Depth = 0;
        uint32_t m_Mips = 0;
        uint32_t m_SampleCount = 0;
        uint32_t m_ArrayCount = 0;
        ResourceState m_State = ResourceState::Undefined;
        TextureUsageFlags m_UsageFlags = TextureUsage::None;
        TextureDimension m_Dimension = TextureDimension::None;
    };
}
