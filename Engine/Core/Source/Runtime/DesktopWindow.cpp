#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Math/Vector2.h"
#include <rgfw/rgfw.h>

#define GET_WINDOW(event) static_cast<FDesktopWindow*>(RGFW_window_getUserPtr(event.win))

namespace Luma
{
    static RGFW_windowFlags getFlags(const FWindowCreateFlags flags)
    {
        RGFW_windowFlags result = RGFW_windowNoResize | RGFW_windowAllowDND;
        if (flags & EWindowCreateBits::Centered) result |= RGFW_windowCenter;
        if (flags & EWindowCreateBits::FullScreen) result |= RGFW_windowFullscreen;
        if (flags & EWindowCreateBits::Resizable) result &= ~RGFW_windowNoResize;
        if (flags & EWindowCreateBits::NoDecoration) result |= RGFW_windowNoBorder;
        if (flags & EWindowCreateBits::Transparent) result |= RGFW_windowTransparent;
        if (flags & EWindowCreateBits::NoDragAndDrop) result &= ~RGFW_windowAllowDND;
        return result;
    }

    bool FDesktopWindow::initialize(const FWindowDesc& windowDesc)
    {
        if (m_Handle) RGFW_window_close(m_Handle);
        m_Handle = RGFW_createWindow(windowDesc.title.data(), 0, 0, windowDesc.width, windowDesc.height, getFlags(windowDesc.flags));
        RGFW_window_setUserPtr(m_Handle, this);

        RGFW_setEventCallback(RGFW_windowResized, [](const RGFW_event* event)
        {
            const auto* window = GET_WINDOW(event->update);
            window->resizedEvent(event->update.w, event->update.h);
        });

        if (!m_Handle) return false;
        RGFW_window_show(m_Handle);
        return true;
    }

    void FDesktopWindow::destroy()
    {
        if (!m_Handle) return;
        RGFW_window_close(m_Handle);
        m_Handle = nullptr;
    }

    void FDesktopWindow::pollEvents()
    {
        RGFW_pollEvents();
    }

    uint32_t FDesktopWindow::getWidth() const
    {
        int32_t width = 0;
        RGFW_window_getSize(m_Handle, &width, nullptr);
        return static_cast<uint32_t>(width);
    }

    uint32_t FDesktopWindow::getHeight() const
    {
        int32_t height = 0;
        RGFW_window_getSize(m_Handle, nullptr, &height);
        return static_cast<uint32_t>(height);
    }

    FVector2u FDesktopWindow::getPosition() const
    {
        FVector2i result;
        RGFW_window_getPosition(m_Handle, &result.x, &result.y);
        return result.as<uint32_t>();
    }

    void FDesktopWindow::setPosition(const FVector2u& position)
    {

    }

    bool FDesktopWindow::hasFocus() const
    {
        return RGFW_window_isInFocus(m_Handle);
    }

    bool FDesktopWindow::isMaximized() const
    {
        return RGFW_window_isMaximized(m_Handle);
    }

    bool FDesktopWindow::isMinimized() const
    {
        return RGFW_window_isMinimized(m_Handle);
    }

    void FDesktopWindow::setFullscreen(bool fullscreen)
    {
        RGFW_window_setFullscreen(m_Handle, fullscreen);
    }

    bool FDesktopWindow::shouldClose() const
    {
        return RGFW_window_shouldClose(m_Handle);
    }

    RGFW_window* FDesktopWindow::getHandle() const
    {
        return m_Handle;
    }

    FString FDesktopWindow::getTitle() const
    {
        return {};
    }

    void FDesktopWindow::setTitle(const FString& title)
    {
        RGFW_window_setName(m_Handle, *title);
    }
}
