#include "Luma/Rendering/Swapchain.h"

namespace Luma::RHI
{
    bool Swapchain::resize(const uint32_t width, const uint32_t height)
    {
        SwapchainDesc swapchainDesc;
        swapchainDesc.device = m_Device;
        swapchainDesc.format = m_Format;
        swapchainDesc.presentMode = m_PresentMode;
        swapchainDesc.width = width;
        swapchainDesc.height = height;
        swapchainDesc.buffering = m_Buffering;
        return initialize(swapchainDesc);
    }

    uint32_t Swapchain::getWidth() const
    {
        return m_Width;
    }

    uint32_t Swapchain::getHeight() const
    {
        return m_Height;
    }

    FRect2u Swapchain::getBounds() const
    {
        return {0, 0, m_Width, m_Height};
    }

    Format Swapchain::getFormat() const
    {
        return m_Format;
    }

    PresentMode Swapchain::getPresentMode() const
    {
        return m_PresentMode;
    }

    SwapchainBuffering Swapchain::getBuffering() const
    {
        return m_Buffering;
    }

    uint32_t Swapchain::getTextureCount() const
    {
        return (uint32_t)m_Buffering;
    }

    Device* Swapchain::getDevice() const
    {
        return m_Device;
    }

    void Swapchain::invalidate()
    {
        m_Valid = false;
    }

    bool Swapchain::isValid() const
    {
        return m_Valid;
    }

    bool Swapchain::hasVSync() const
    {
        return m_PresentMode == PresentMode::Fifo;
    }

    SwapchainDesc Swapchain::getDesc() const
    {
        SwapchainDesc desc;
        desc.device = m_Device;
        desc.width = m_Width;
        desc.height = m_Height;
        desc.buffering = m_Buffering;
        desc.format = m_Format;
        desc.presentMode = m_PresentMode;
        return desc;
    }
}
