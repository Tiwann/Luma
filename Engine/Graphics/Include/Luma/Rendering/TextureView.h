#pragma once
#include "ComponentMapping.h"
#include "TextureAspect.h"
#include "Luma/Math/Rect2.h"
#include "Luma/Runtime/Format.h"

namespace Luma::RHI
{
    struct Device;
    struct Texture;

    struct TextureViewDesc
    {
        Device* device = nullptr;
        const Texture* texture = nullptr;
        Format format = Format::None;
        TextureAspectFlags aspectFlags = TextureAspect::None;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 0;
        uint32_t startMipIndex = 0;
        uint32_t mipCount = 0;
        uint32_t startArrayIndex = 0;
        uint32_t arrayCount = 0;
        ComponentMapping mapping = ComponentMapping();
    };

    struct TextureView
    {
        TextureView() = default;
        virtual ~TextureView() = default;
        virtual bool initialize(const TextureViewDesc& textureViewDesc) = 0;
        virtual void destroy() = 0;
        virtual void setName(StringView name) {}
        const Texture* getTexture() const { return m_Texture; }
        Format getFormat() const { return m_Format; }
        TextureAspectFlags getAspectFlags() const { return m_AspectFlags; }
        uint32_t getWidth() const { return m_Width; }
        uint32_t getHeight() const { return m_Height; }
        uint32_t getDepth() const { return m_Depth; }
        uint32_t getStartMipIndex() const { return m_StartMipIndex; }
        uint32_t getMipCount() const { return m_MipCount; }
        uint32_t getStartArrayIndex() const { return m_StartArrayIndex; }
        uint32_t getArrayCount() const { return m_ArrayCount; }
        FRect2u getArea() const { return FRect2u(0, 0, m_Width, m_Height); }
    protected:
        const Texture* m_Texture = nullptr;
        Format m_Format = Format::None;
        TextureAspectFlags m_AspectFlags = TextureAspect::None;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_Depth = 0;
        uint32_t m_StartMipIndex = 0;
        uint32_t m_MipCount = 0;
        uint32_t m_StartArrayIndex = 0;
        uint32_t m_ArrayCount = 0;
    };
}
