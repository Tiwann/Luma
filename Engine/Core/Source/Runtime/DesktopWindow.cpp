#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Math/Vector2.h"
#include <rgfw/rgfw.h>

#include "Luma/Input/Immediate.h"
#include "Luma/Input/Keyboard.h"
#include "Luma/Input/Mouse.h"

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

    static EKey getKeyFromRGFW(RGFW_key key)
    {
        switch (key)
        {
        case RGFW_keyNULL: return EKey::None;
        case RGFW_keyEscape: return EKey::Escape;
        case RGFW_keyBacktick: return EKey::None;
        case RGFW_key0: return EKey::None;
            case RGFW_key1: return EKey::None;
            case RGFW_key2: return EKey::None;
            case RGFW_key3: return EKey::None;
            case RGFW_key4: return EKey::None;
            case RGFW_key5: return EKey::None;
            case RGFW_key6: return EKey::None;
            case RGFW_key7: return EKey::None;
            case RGFW_key8: return EKey::None;
            case RGFW_key9: return EKey::None;
            case RGFW_keyMinus: return EKey::Minus;
            case RGFW_keyEqual: return EKey::Equals;
            case RGFW_keyBackSpace: return EKey::Backspace;
            case RGFW_keyTab: return EKey::Tab;
            case RGFW_keySpace: return EKey::Space;
            case RGFW_keyA: return EKey::A;
            case RGFW_keyB: return EKey::B;
            case RGFW_keyC: return EKey::C;
            case RGFW_keyD: return EKey::D;
            case RGFW_keyE: return EKey::E;
            case RGFW_keyF: return EKey::F;
            case RGFW_keyG: return EKey::G;
            case RGFW_keyH: return EKey::H;
            case RGFW_keyI: return EKey::I;
            case RGFW_keyJ: return EKey::J;
            case RGFW_keyK: return EKey::K;
            case RGFW_keyL: return EKey::L;
            case RGFW_keyM: return EKey::M;
            case RGFW_keyN: return EKey::N;
            case RGFW_keyO: return EKey::O;
            case RGFW_keyP: return EKey::P;
            case RGFW_keyQ: return EKey::Q;
            case RGFW_keyR: return EKey::R;
            case RGFW_keyS: return EKey::S;
            case RGFW_keyT: return EKey::T;
            case RGFW_keyU: return EKey::U;
            case RGFW_keyV: return EKey::V;
            case RGFW_keyW: return EKey::W;
            case RGFW_keyX: return EKey::X;
            case RGFW_keyY: return EKey::Y;
            case RGFW_keyZ: return EKey::Z;
            case RGFW_keyPeriod: return EKey::Period;
            case RGFW_keyComma: return EKey::Comma;
            case RGFW_keySlash: return EKey::Slash;
            case RGFW_keyBracket: return EKey::LeftBracket;
            case RGFW_keyCloseBracket: return EKey::Right;
            case RGFW_keySemicolon: return EKey::Semicolon;
            case RGFW_keyApostrophe: return EKey::Apostrophe;
            case RGFW_keyBackSlash: return EKey::Backslash;
            case RGFW_keyReturn: return EKey::Enter;
            case RGFW_keyDelete: return EKey::Delete;
            case RGFW_keyF1: return EKey::F1;
            case RGFW_keyF2: return EKey::F2;
            case RGFW_keyF3: return EKey::F3;
            case RGFW_keyF4: return EKey::F4;
            case RGFW_keyF5: return EKey::F5;
            case RGFW_keyF6: return EKey::F6;
            case RGFW_keyF7: return EKey::F7;
            case RGFW_keyF8: return EKey::F8;
            case RGFW_keyF9: return EKey::F9;
            case RGFW_keyF10: return EKey::F10;
            case RGFW_keyF11: return EKey::F11;
            case RGFW_keyF12: return EKey::F12;
            case RGFW_keyF13: return EKey::F13;
            case RGFW_keyF14: return EKey::F14;
            case RGFW_keyF15: return EKey::F15;
            case RGFW_keyF16: return EKey::F16;
            case RGFW_keyF17: return EKey::F17;
            case RGFW_keyF18: return EKey::F18;
            case RGFW_keyF19: return EKey::F19;
            case RGFW_keyF20: return EKey::F20;
            case RGFW_keyF21: return EKey::F21;
            case RGFW_keyF22: return EKey::F22;
            case RGFW_keyF23: return EKey::F23;
            case RGFW_keyF24: return EKey::F24;
            case RGFW_keyF25: return EKey::F25;
            case RGFW_keyCapsLock: return EKey::CapsLock;
            case RGFW_keyShiftL: return EKey::LeftShift;
            case RGFW_keyControlL: return EKey::LeftCtrl;
            case RGFW_keyAltL: return EKey::LeftAlt;
            case RGFW_keySuperL: return EKey::LeftSuper;
            case RGFW_keyShiftR: return EKey::RightShift;
            case RGFW_keyControlR: return EKey::RightCtrl;
            case RGFW_keyAltR: return EKey::RightAlt;
            case RGFW_keySuperR: return EKey::RightSuper;
            case RGFW_keyUp: return EKey::Up;
            case RGFW_keyDown: return EKey::Down;
            case RGFW_keyLeft: return EKey::Left;
            case RGFW_keyRight: return EKey::Right;
            case RGFW_keyInsert: return EKey::Insert;
            case RGFW_keyMenu: return EKey::None;
            case RGFW_keyEnd: return EKey::End;
            case RGFW_keyHome: return EKey::Home;
            case RGFW_keyPageUp: return EKey::PageUp;
            case RGFW_keyPageDown: return EKey::PageDown;
            case RGFW_keyNumLock: return EKey::NumLock;
            case RGFW_keyPadSlash: return EKey::NumpadDiv;
            case RGFW_keyPadMultiply: return EKey::NumpadMul;
            case RGFW_keyPadPlus: return EKey::NumpadAdd;
            case RGFW_keyPadMinus: return EKey::NumpadSub;
            case RGFW_keyPadEqual: return EKey::NumpadEnter;
            case RGFW_keyPad1: return EKey::Numpad1;
            case RGFW_keyPad2: return EKey::Numpad2;
            case RGFW_keyPad3: return EKey::Numpad3;
            case RGFW_keyPad4: return EKey::Numpad4;
            case RGFW_keyPad5: return EKey::Numpad5;
            case RGFW_keyPad6: return EKey::Numpad6;
            case RGFW_keyPad7: return EKey::Numpad7;
            case RGFW_keyPad8: return EKey::Numpad8;
            case RGFW_keyPad9: return EKey::Numpad9;
            case RGFW_keyPad0: return EKey::Numpad0;
            case RGFW_keyPadPeriod: return EKey::NumpadPeriod;
            case RGFW_keyPadReturn: return EKey::None;
            case RGFW_keyScrollLock: return EKey::ScrollLock;
            case RGFW_keyPrintScreen: return EKey::PrintScreen;
            case RGFW_keyPause: return EKey::Pause;
            case RGFW_keyWorld1: return EKey::None;
            case RGFW_keyWorld2: return EKey::None;
            default: return EKey::None;
        }
    }

    static EMouseButton getMouseButtonFromRGFW(RGFW_mouseButton button)
    {
        switch (button)
        {
            case RGFW_mouseLeft: return EMouseButton::Left;
            case RGFW_mouseMiddle: return EMouseButton::Middle;
            case RGFW_mouseRight: return EMouseButton::Right;
            default: return EMouseButton::None;
        }
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

        RGFW_setEventCallback(RGFW_windowMoved, [](const RGFW_event* event)
        {
            const auto* window = GET_WINDOW(event->update);
            window->movedEvent(event->update.x, event->update.y);
        });

        RGFW_setEventCallback(RGFW_windowFocusIn, [](const RGFW_event* event)
        {
            const auto* window = GET_WINDOW(event->focus);
            window->focusedEvent(true);
        });

        RGFW_setEventCallback(RGFW_windowFocusOut, [](const RGFW_event* event)
        {
            const auto* window = GET_WINDOW(event->focus);
            window->focusedEvent(false);
        });

        RGFW_setEventCallback(RGFW_windowClose, [](const RGFW_event* event)
        {
            const auto* window = GET_WINDOW(event->common);
            window->closedEvent();
        });

        RGFW_setEventCallback(RGFW_windowMaximized, [](const RGFW_event* event)
        {
            const auto* window = GET_WINDOW(event->common);
            window->maximizedEvent();
        });

        RGFW_setEventCallback(RGFW_windowMinimized, [](const RGFW_event* event)
        {
            const auto* window = GET_WINDOW(event->common);
            window->minimizedEvent();
        });

        RGFW_setEventCallback(RGFW_keyPressed, [](const RGFW_event* event)
        {
            FInput::updateKeyState(getKeyFromRGFW(event->key.value), EInputState::Pressed);
        });

        RGFW_setEventCallback(RGFW_keyReleased, [](const RGFW_event* event)
        {
            FInput::updateKeyState(getKeyFromRGFW(event->key.value), EInputState::Released);
        });

        RGFW_setEventCallback(RGFW_mouseButtonPressed, [](const RGFW_event* event)
        {
            FInput::updateMouseButtonState(getMouseButtonFromRGFW(event->button.value), EInputState::Pressed);
        });

        RGFW_setEventCallback(RGFW_mouseButtonReleased, [](const RGFW_event* event)
        {
            FInput::updateMouseButtonState(getMouseButtonFromRGFW(event->button.value), EInputState::Released);
        });

        RGFW_setEventCallback(RGFW_mousePosChanged, [](const RGFW_event* event)
        {
            FInput::updateMousePosition(FVector2d(event->mouse.x, event->mouse.y));
        });

        if (!m_Handle) return false;
        RGFW_window_show(m_Handle);

        m_Title = windowDesc.title;
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
        FVector2<int32_t> result;
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
        return m_Title;
    }

    void FDesktopWindow::setTitle(const FString& title)
    {
        RGFW_window_setName(m_Handle, *title);
        m_Title = title;
    }
}

#undef GET_WINDOW