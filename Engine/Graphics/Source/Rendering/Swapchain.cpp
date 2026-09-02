#include "Luma/Rendering/Swapchain.h"

namespace Luma
{
    bool ISwapchain::resize(const uint32_t width, const uint32_t height)
    {
        FSwapchainDesc swapchainDesc;
        swapchainDesc.device = m_Device;
        swapchainDesc.format = m_Format;
        swapchainDesc.presentMode = m_PresentMode;
        swapchainDesc.width = width;
        swapchainDesc.height = height;
        swapchainDesc.buffering = m_Buffering;
        return initialize(swapchainDesc);
    }

    uint32_t ISwapchain::getWidth() const
    {
        return m_Width;
    }

    uint32_t ISwapchain::getHeight() const
    {
        return m_Height;
    }

    EFormat ISwapchain::getFormat() const
    {
        return m_Format;
    }

    EPresentMode ISwapchain::getPresentMode() const
    {
        return m_PresentMode;
    }

    uint32_t ISwapchain::getTextureCount() const
    {
        return (uint32_t)m_Buffering;
    }

    IRenderDevice* ISwapchain::getDevice() const
    {
        return m_Device;
    }

    void ISwapchain::invalidate()
    {
        m_Valid = false;
    }

    bool ISwapchain::isValid() const
    {
        return m_Valid;
    }

    bool ISwapchain::hasVSync() const
    {
        return m_PresentMode == EPresentMode::Fifo;
    }
}
