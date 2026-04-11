#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Math/Vector2.h"
#include <rgfw/rgfw.h>

namespace luma
{
    struct IWindow::Impl
    {
        RGFW_window* window = nullptr;
    };

    FDesktopWindow::FDesktopWindow()
    {
        m_Pimpl = new Impl();
    }

    FDesktopWindow::~FDesktopWindow()
    {
        delete m_Pimpl;
    }

    bool FDesktopWindow::initialize(const FWindowCreateInfo& createInfo)
    {
        if (m_Pimpl->window) RGFW_window_close(m_Pimpl->window);
        m_Pimpl->window = RGFW_createWindow(createInfo.title.data(), 0, 0, createInfo.width, createInfo.height, RGFW_windowCenter);
        if (!m_Pimpl->window) return false;
        RGFW_window_show(m_Pimpl->window);
        return true;
    }

    void FDesktopWindow::destroy()
    {
        RGFW_window_close(m_Pimpl->window);
    }

    void FDesktopWindow::pollEvents()
    {
        RGFW_pollEvents();
    }

    uint32_t FDesktopWindow::getWidth() const
    {
        int32_t width = 0;
        RGFW_window_getSize(m_Pimpl->window, &width, nullptr);
        return static_cast<uint32_t>(width);
    }

    uint32_t FDesktopWindow::getHeight() const
    {
        int32_t height = 0;
        RGFW_window_getSize(m_Pimpl->window, nullptr, &height);
        return static_cast<uint32_t>(height);
    }

    FVector2u FDesktopWindow::getPosition() const
    {
        FVector2i result;
        RGFW_window_getPosition(m_Pimpl->window, &result.x, &result.y);
        return result.as<uint32_t>();
    }

    void FDesktopWindow::setPosition(const FVector2u& position)
    {

    }

    bool FDesktopWindow::hasFocus() const
    {
        return RGFW_window_isInFocus(m_Pimpl->window);
    }

    bool FDesktopWindow::isMaximized() const
    {
        return RGFW_window_isMaximized(m_Pimpl->window);
    }

    bool FDesktopWindow::isMinimized() const
    {
        return RGFW_window_isMinimized(m_Pimpl->window);
    }

    void FDesktopWindow::setFullscreen(bool fullscreen)
    {
        RGFW_window_setFullscreen(m_Pimpl->window, fullscreen);
    }

    bool FDesktopWindow::shouldClose() const
    {
        return RGFW_window_shouldClose(m_Pimpl->window);
    }
}
