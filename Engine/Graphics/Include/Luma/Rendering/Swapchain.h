#pragma once
#include <cstdint>
#include "PresentMode.h"
#include "SwpchainBuffering.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Math/Rect2.h"
#include "Luma/Runtime/Format.h"


namespace Luma::RHI
{
    struct Device;
    struct Texture;
    struct TextureView;

    struct SwapchainDesc
    {
        Device* device = nullptr;
        Format format = Format::None;
        SwapchainBuffering buffering = SwapchainBuffering::None;
        PresentMode presentMode = PresentMode::Unknown;
        uint32_t width = 0;
        uint32_t height = 0;
    };

    struct Swapchain
    {
        Swapchain() = default;
        virtual ~Swapchain() = default;

        virtual bool initialize(const SwapchainDesc& swapchainDesc) = 0;
        virtual void destroy() = 0;
        virtual bool resize(uint32_t width, uint32_t height);

        uint32_t getWidth() const;
        uint32_t getHeight() const;
        FRect2u getBounds() const;
        Format getFormat() const;
        PresentMode getPresentMode() const;
        SwapchainBuffering getBuffering() const;
        uint32_t getTextureCount() const;

        Device* getDevice() const;

        void invalidate();
        virtual bool isValid() const;
        bool hasVSync() const;

        virtual Texture* getTexture(uint32_t index) { return nullptr; }
        virtual TextureView* getTextureView(uint32_t index) { return nullptr; }

        virtual void setName(StringView name) {}

        SwapchainDesc getDesc() const;
    protected:
        Device* m_Device = nullptr;
        Format m_Format = Format::None;
        SwapchainBuffering m_Buffering = SwapchainBuffering::None;
        PresentMode m_PresentMode = PresentMode::Unknown;
        uint32_t m_Width = 0, m_Height = 0;
        bool m_Valid = true;
    };
}
