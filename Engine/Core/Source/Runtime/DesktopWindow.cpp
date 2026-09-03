#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Input/Immediate.h"
#include "Luma/Input/Keyboard.h"
#include "Luma/Input/Mouse.h"
#include <GLFW/glfw3.h>

#define GET_WINDOW(x) static_cast<FDesktopWindow*>(glfwGetWindowUserPointer(x))

namespace Luma
{
    /*static RGFW_windowFlags getFlags(const FWindowCreateFlags flags)
    {
        RGFW_windowFlags result = RGFW_windowNoResize | RGFW_windowAllowDND;
        if (flags & EWindowCreateBits::Centered) result |= RGFW_windowCenter;
        if (flags & EWindowCreateBits::FullScreen) result |= RGFW_windowFullscreen;
        if (flags & EWindowCreateBits::Resizable) result &= ~RGFW_windowNoResize;
        if (flags & EWindowCreateBits::NoDecoration) result |= RGFW_windowNoBorder;
        if (flags & EWindowCreateBits::Transparent) result |= RGFW_windowTransparent;
        if (flags & EWindowCreateBits::NoDragAndDrop) result &= ~RGFW_windowAllowDND;
        return result;
    }*/

    static EKey getKeyFromGLFW(int key)
    {
        switch (key)
        {
        case GLFW_KEY_UNKNOWN: return EKey::None;
        case GLFW_KEY_ESCAPE: return EKey::Escape;
        case GLFW_KEY_0: return EKey::None;
            case GLFW_KEY_1: return EKey::None;
            case GLFW_KEY_2: return EKey::None;
            case GLFW_KEY_3: return EKey::None;
            case GLFW_KEY_4: return EKey::None;
            case GLFW_KEY_5: return EKey::None;
            case GLFW_KEY_6: return EKey::None;
            case GLFW_KEY_7: return EKey::None;
            case GLFW_KEY_8: return EKey::None;
            case GLFW_KEY_9: return EKey::None;
            case GLFW_KEY_MINUS: return EKey::Minus;
            case GLFW_KEY_EQUAL: return EKey::Equals;
            case GLFW_KEY_BACKSPACE: return EKey::Backspace;
            case GLFW_KEY_TAB: return EKey::Tab;
            case GLFW_KEY_SPACE: return EKey::Space;
            case GLFW_KEY_A: return EKey::A;
            case GLFW_KEY_B: return EKey::B;
            case GLFW_KEY_C: return EKey::C;
            case GLFW_KEY_D: return EKey::D;
            case GLFW_KEY_E: return EKey::E;
            case GLFW_KEY_F: return EKey::F;
            case GLFW_KEY_G: return EKey::G;
            case GLFW_KEY_H: return EKey::H;
            case GLFW_KEY_I: return EKey::I;
            case GLFW_KEY_J: return EKey::J;
            case GLFW_KEY_K: return EKey::K;
            case GLFW_KEY_L: return EKey::L;
            case GLFW_KEY_M: return EKey::M;
            case GLFW_KEY_N: return EKey::N;
            case GLFW_KEY_O: return EKey::O;
            case GLFW_KEY_P: return EKey::P;
            case GLFW_KEY_Q: return EKey::Q;
            case GLFW_KEY_R: return EKey::R;
            case GLFW_KEY_S: return EKey::S;
            case GLFW_KEY_T: return EKey::T;
            case GLFW_KEY_U: return EKey::U;
            case GLFW_KEY_V: return EKey::V;
            case GLFW_KEY_W: return EKey::W;
            case GLFW_KEY_X: return EKey::X;
            case GLFW_KEY_Y: return EKey::Y;
            case GLFW_KEY_Z: return EKey::Z;
            case GLFW_KEY_PERIOD: return EKey::Period;
            case GLFW_KEY_COMMA: return EKey::Comma;
            case GLFW_KEY_SLASH: return EKey::Slash;
            case GLFW_KEY_LEFT_BRACKET: return EKey::LeftBracket;
            case GLFW_KEY_RIGHT_BRACKET: return EKey::Right;
            case GLFW_KEY_SEMICOLON: return EKey::Semicolon;
            case GLFW_KEY_APOSTROPHE: return EKey::Apostrophe;
            case GLFW_KEY_BACKSLASH: return EKey::Backslash;
            case GLFW_KEY_ENTER: return EKey::Enter;
            case GLFW_KEY_DELETE: return EKey::Delete;
            case GLFW_KEY_F1: return EKey::F1;
            case GLFW_KEY_F2: return EKey::F2;
            case GLFW_KEY_F3: return EKey::F3;
            case GLFW_KEY_F4: return EKey::F4;
            case GLFW_KEY_F5: return EKey::F5;
            case GLFW_KEY_F6: return EKey::F6;
            case GLFW_KEY_F7: return EKey::F7;
            case GLFW_KEY_F8: return EKey::F8;
            case GLFW_KEY_F9: return EKey::F9;
            case GLFW_KEY_F10: return EKey::F10;
            case GLFW_KEY_F11: return EKey::F11;
            case GLFW_KEY_F12: return EKey::F12;
            case GLFW_KEY_F13: return EKey::F13;
            case GLFW_KEY_F14: return EKey::F14;
            case GLFW_KEY_F15: return EKey::F15;
            case GLFW_KEY_F16: return EKey::F16;
            case GLFW_KEY_F17: return EKey::F17;
            case GLFW_KEY_F18: return EKey::F18;
            case GLFW_KEY_F19: return EKey::F19;
            case GLFW_KEY_F20: return EKey::F20;
            case GLFW_KEY_F21: return EKey::F21;
            case GLFW_KEY_F22: return EKey::F22;
            case GLFW_KEY_F23: return EKey::F23;
            case GLFW_KEY_F24: return EKey::F24;
            case GLFW_KEY_F25: return EKey::F25;
            case GLFW_KEY_CAPS_LOCK: return EKey::CapsLock;
            case GLFW_KEY_LEFT_SHIFT: return EKey::LeftShift;
            case GLFW_KEY_LEFT_CONTROL: return EKey::LeftCtrl;
            case GLFW_KEY_LEFT_ALT: return EKey::LeftAlt;
            case GLFW_KEY_LEFT_SUPER: return EKey::LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT: return EKey::RightShift;
            case GLFW_KEY_RIGHT_CONTROL: return EKey::RightCtrl;
            case GLFW_KEY_RIGHT_ALT: return EKey::RightAlt;
            case GLFW_KEY_RIGHT_SUPER: return EKey::RightSuper;
            case GLFW_KEY_UP: return EKey::Up;
            case GLFW_KEY_DOWN: return EKey::Down;
            case GLFW_KEY_LEFT: return EKey::Left;
            case GLFW_KEY_RIGHT: return EKey::Right;
            case GLFW_KEY_INSERT: return EKey::Insert;
            case GLFW_KEY_MENU: return EKey::None;
            case GLFW_KEY_END: return EKey::End;
            case GLFW_KEY_HOME: return EKey::Home;
            case GLFW_KEY_PAGE_UP: return EKey::PageUp;
            case GLFW_KEY_PAGE_DOWN: return EKey::PageDown;
            case GLFW_KEY_NUM_LOCK: return EKey::NumLock;
            case GLFW_KEY_KP_DIVIDE: return EKey::NumpadDiv;
            case GLFW_KEY_KP_MULTIPLY: return EKey::NumpadMul;
            case GLFW_KEY_KP_ADD: return EKey::NumpadAdd;
            case GLFW_KEY_KP_SUBTRACT: return EKey::NumpadSub;
            case GLFW_KEY_KP_1: return EKey::Numpad1;
            case GLFW_KEY_KP_2: return EKey::Numpad2;
            case GLFW_KEY_KP_3: return EKey::Numpad3;
            case GLFW_KEY_KP_4: return EKey::Numpad4;
            case GLFW_KEY_KP_5: return EKey::Numpad5;
            case GLFW_KEY_KP_6: return EKey::Numpad6;
            case GLFW_KEY_KP_7: return EKey::Numpad7;
            case GLFW_KEY_KP_8: return EKey::Numpad8;
            case GLFW_KEY_KP_9: return EKey::Numpad9;
            case GLFW_KEY_KP_0: return EKey::Numpad0;
            case GLFW_KEY_KP_DECIMAL: return EKey::NumpadPeriod;
            case GLFW_KEY_KP_ENTER: return EKey::NumpadEnter;
            case GLFW_KEY_SCROLL_LOCK: return EKey::ScrollLock;
            case GLFW_KEY_PRINT_SCREEN: return EKey::PrintScreen;
            case GLFW_KEY_PAUSE: return EKey::Pause;
            case GLFW_KEY_WORLD_1: return EKey::None;
            case GLFW_KEY_WORLD_2: return EKey::None;
            default: return EKey::None;
        }
    }

    static EMouseButton getMouseButtonFromGLFW(int button)
    {
        switch (button)
        {
            case GLFW_MOUSE_BUTTON_LEFT: return EMouseButton::Left;
            case GLFW_MOUSE_BUTTON_MIDDLE: return EMouseButton::Middle;
            case GLFW_MOUSE_BUTTON_RIGHT: return EMouseButton::Right;
            default: return EMouseButton::None;
        }
    }

    bool FDesktopWindow::initialize(const FWindowDesc& windowDesc)
    {
        glfwInit();
        if (m_Handle) glfwDestroyWindow(m_Handle);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Handle = glfwCreateWindow(windowDesc.width, windowDesc.height, *windowDesc.title, nullptr, nullptr);
        if (!m_Handle) return false;
        glfwSetWindowUserPointer(m_Handle, this);

        glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* w, const int width, const int height)
        {
            auto* window = GET_WINDOW(w);
            window->resizedEvent(width, height);
        });

        glfwSetWindowPosCallback(m_Handle, [](GLFWwindow* w, const int x, const int y)
        {
            auto* window = GET_WINDOW(w);
            window->movedEvent(x, y);
        });

        glfwSetWindowFocusCallback(m_Handle, [](GLFWwindow* w, int focused)
        {
            auto* window = GET_WINDOW(w);
            window->m_Focused = focused;
            window->focusedEvent(static_cast<bool>(focused));
        });
        
        glfwSetWindowMaximizeCallback(m_Handle, [](GLFWwindow* w, int maximized)
        {
            auto* window = GET_WINDOW(w);
            window->m_Maximized = maximized;
            if (maximized) window->maximizedEvent();
        });

        glfwSetWindowIconifyCallback(m_Handle, [](GLFWwindow* w, int iconified)
        {
            auto* window = GET_WINDOW(w);
            window->m_Minimized = iconified;
            if (iconified) window->minimizedEvent();
        });

        glfwSetKeyCallback(m_Handle, [](GLFWwindow*, int key, int, int action, int)
        {
            if (action == GLFW_PRESS)
                FInput::updateKeyState(getKeyFromGLFW(key), EInputState::Pressed);

            if (action == GLFW_RELEASE)
                FInput::updateKeyState(getKeyFromGLFW(key), EInputState::Released);
        });

        glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow*, int button, int action, int)
        {
            if (action == GLFW_PRESS)
                FInput::updateMouseButtonState(getMouseButtonFromGLFW(button), EInputState::Pressed);
            if (action == GLFW_RELEASE)
                FInput::updateMouseButtonState(getMouseButtonFromGLFW(button), EInputState::Released);
        });

        glfwSetCursorPosCallback(m_Handle, [](GLFWwindow*, double x, double y)
        {
            FInput::updateMousePosition(FVector2d(x, y));
        });

        glfwShowWindow(m_Handle);
        m_Title = windowDesc.title;
        return true;
    }

    void FDesktopWindow::destroy()
    {
        if (!m_Handle) return;
        glfwDestroyWindow(m_Handle);
        m_Handle = nullptr;
    }

    void FDesktopWindow::pollEvents()
    {
        glfwPollEvents();
    }

    uint32_t FDesktopWindow::getWidth() const
    {
        int32_t width = 0;
        glfwGetWindowSize(m_Handle, &width, nullptr);
        return static_cast<uint32_t>(width);
    }

    uint32_t FDesktopWindow::getHeight() const
    {
        int32_t height = 0;
        glfwGetWindowSize(m_Handle, nullptr, &height);
        return static_cast<uint32_t>(height);
    }

    FVector2u FDesktopWindow::getPosition() const
    {
        FVector2<int32_t> result;
        glfwGetWindowPos(m_Handle, &result.x, &result.y);
        return result.as<uint32_t>();
    }

    void FDesktopWindow::setPosition(const FVector2u& position)
    {
        glfwSetWindowPos(m_Handle, position.x, position.y);
    }

    bool FDesktopWindow::hasFocus() const
    {
        return m_Focused;
    }

    bool FDesktopWindow::isMaximized() const
    {
        return m_Maximized;
    }

    bool FDesktopWindow::isMinimized() const
    {
        return m_Minimized;
    }

    void FDesktopWindow::setFullscreen(bool fullscreen)
    {

    }

    bool FDesktopWindow::isAvailable() const
    {
        return !isMinimized();
    }

    bool FDesktopWindow::shouldClose() const
    {
        return glfwWindowShouldClose(m_Handle);
    }

    GLFWwindow* FDesktopWindow::getHandle() const
    {
        return m_Handle;
    }

    FString FDesktopWindow::getTitle() const
    {
        return m_Title;
    }

    void FDesktopWindow::setTitle(const FString& title)
    {
        glfwSetWindowTitle(m_Handle, *title);
        m_Title = title;
    }
}

#undef GET_WINDOW