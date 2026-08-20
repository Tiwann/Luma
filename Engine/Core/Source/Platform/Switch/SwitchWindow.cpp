#include "Luma/Runtime/SwitchWindow.h"
#include <switch/services/applet.h>
#include <switch/display/native_window.h>

namespace Luma
{
    static ESwitchHandleMode toLuma(AppletOperationMode mode)
    {
        switch (mode)
        {
        case AppletOperationMode_Handheld: return ESwitchHandleMode::Handled;
        case AppletOperationMode_Console: return ESwitchHandleMode::Docked;
        }
        return ESwitchHandleMode::Handled;
    }

    static bool isFocused(AppletFocusState state)
    {
        return state == AppletFocusState_InFocus;
    }

    bool FSwitchWindow::initialize(const FWindowDesc& windowDesc)
    {
        m_Handle = nwindowGetDefault();
        if (!m_Handle) return false;
        setHandleMode(toLuma(appletGetOperationMode()));
        return true;
    }

    void FSwitchWindow::destroy()
    {
        closedEvent();
        nwindowClose(m_Handle);
        m_Handle = nullptr;
    }

    void FSwitchWindow::pollEvents()
    {
        const ESwitchHandleMode mode = toLuma(appletGetOperationMode());
        const bool focus = isFocused(appletGetFocusState());

        if (mode != m_HandleMode)
            setHandleMode(mode);

        if (focus != m_Focus)
            setFocus(focus);
    }

    uint32_t FSwitchWindow::getWidth() const
    {
        return m_Width;
    }

    uint32_t FSwitchWindow::getHeight() const
    {
        return m_Height;
    }

    bool FSwitchWindow::shouldClose() const
    {
        return appletMainLoop();
    }

    NWindow* FSwitchWindow::getHandle() const
    {
        return m_Handle;
    }

    ESwitchHandleMode FSwitchWindow::getHandleMode() const
    {
        return m_HandleMode;
    }

    void FSwitchWindow::setHandleMode(ESwitchHandleMode mode)
    {
        switch (mode)
        {
        case ESwitchHandleMode::Handled:
            {
                m_Width = 1280;
                m_Height = 720;
                resizedEvent(m_Width, m_Height);
                break;
            }
        case ESwitchHandleMode::Docked:
            {
                m_Width = 1920;
                m_Height = 1080;
                resizedEvent(m_Width, m_Height);
                break;
            }
        }
        m_HandleMode = mode;
    }

    void FSwitchWindow::setFocus(bool focus)
    {
        focusedEvent(focus);
        m_Focus = focus;
    }
}
