#pragma once
#include "Luma/Runtime/Format.h"
#include "PresentMode.h"
#include "SwpchainBuffering.h"
#include <cstdint>

namespace Luma
{
    struct IRenderDevice;
    struct ITexture;
    struct ITextureView;

    struct FSwapchainDesc
    {
        IRenderDevice* device = nullptr;
        EFormat format = EFormat::None;
        ESwapchainBuffering buffering = ESwapchainBuffering::None;
        EPresentMode presentMode = EPresentMode::Unknown;
        uint32_t width = 0;
        uint32_t height = 0;
    };

    struct ISwapchain
    {
        ISwapchain() = default;
        virtual ~ISwapchain() = default;

        virtual bool initialize(const FSwapchainDesc& swapchainDesc) = 0;
        virtual void destroy() = 0;

        bool resize(uint32_t width, uint32_t height);
        uint32_t getWidth() const;
        uint32_t getHeight() const;
        EFormat getFormat() const;
        EPresentMode getPresentMode() const;
        uint32_t getImageCount() const;

        IRenderDevice* getDevice() const;

        void invalidate();
        virtual bool isValid() const;
        bool hasVSync() const;

        virtual ITexture* getTexture(uint32_t index) { return nullptr; }
        virtual ITextureView* getTextureView(uint32_t index) { return nullptr; }
    protected:
        IRenderDevice* m_Device = nullptr;
        EFormat m_ImageFormat = EFormat::None;
        ESwapchainBuffering m_Buffering = ESwapchainBuffering::None;
        EPresentMode m_ImagePresentMode = EPresentMode::Unknown;
        uint32_t m_ImageWidth = 0, m_ImageHeight = 0;
        bool m_HasVSync = false;
        bool m_Valid = true;
    };
}
