#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Math/Rect2.h"
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

    bool FDesktopWindow::shouldClose() const
    {
        return RGFW_window_shouldClose(m_Pimpl->window);
    }
}
