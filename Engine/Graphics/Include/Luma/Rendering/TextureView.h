#pragma once
#include "TextureAspect.h"
#include "ComponentMapping.h"
#include "Luma/Runtime/Format.h"
#include "Luma/Containers/StringView.h"
#include <cstdint>

namespace Luma
{
    struct IRenderDevice;
    struct ITexture;

    struct FTextureViewDesc
    {
        IRenderDevice* device = nullptr;
        const ITexture* texture = nullptr;
        EFormat format = EFormat::None;
        FTextureAspectFlags aspectFlags = ETextureAspectBits::None;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 0;
        uint32_t baseMipLevel = 0;
        uint32_t mipCount = 0;
        uint32_t baseArray = 0;
        uint32_t arrayCount = 0;
        FComponentMapping mapping = FComponentMapping();
    };

    struct ITextureView
    {
        ITextureView() = default;
        virtual ~ITextureView() = default;
        virtual bool initialize(const FTextureViewDesc& textureViewDesc) = 0;
        virtual void destroy() = 0;
        virtual void setName(FStringView name) {}
        const ITexture* getTexture() const { return m_Texture; }
        EFormat getFormat() const { return m_Format; }
        FTextureAspectFlags getAspectFlags() const { return m_AspectFlags; }
        uint32_t getWidth() const { return m_Width; }
        uint32_t getHeight() const { return m_Height; }
        uint32_t getDepth() const { return m_Depth; }
        uint32_t getBaseMipLevel() const { return m_BaseMipLevel; }
        uint32_t getMipCount() const { return m_MipCount; }
    protected:
        const ITexture* m_Texture = nullptr;
        EFormat m_Format = EFormat::None;
        FTextureAspectFlags m_AspectFlags = ETextureAspectBits::None;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_Depth = 0;
        uint32_t m_BaseMipLevel = 0;
        uint32_t m_MipCount = 0;
    };
}
