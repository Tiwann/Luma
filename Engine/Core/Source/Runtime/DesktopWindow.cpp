#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Input/Immediate.h"
#include "Luma/Input/Keyboard.h"
#include "Luma/Input/Mouse.h"
#include <GLFW/glfw3.h>

#define GET_WINDOW(x) static_cast<DesktopWindow*>(glfwGetWindowUserPointer(x))

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

    static Key getKeyFromGLFW(int key)
    {
        switch (key)
        {
        case GLFW_KEY_UNKNOWN: return Key::None;
        case GLFW_KEY_ESCAPE: return Key::Escape;
        case GLFW_KEY_0: return Key::None;
            case GLFW_KEY_1: return Key::None;
            case GLFW_KEY_2: return Key::None;
            case GLFW_KEY_3: return Key::None;
            case GLFW_KEY_4: return Key::None;
            case GLFW_KEY_5: return Key::None;
            case GLFW_KEY_6: return Key::None;
            case GLFW_KEY_7: return Key::None;
            case GLFW_KEY_8: return Key::None;
            case GLFW_KEY_9: return Key::None;
            case GLFW_KEY_MINUS: return Key::Minus;
            case GLFW_KEY_EQUAL: return Key::Equals;
            case GLFW_KEY_BACKSPACE: return Key::Backspace;
            case GLFW_KEY_TAB: return Key::Tab;
            case GLFW_KEY_SPACE: return Key::Space;
            case GLFW_KEY_A: return Key::A;
            case GLFW_KEY_B: return Key::B;
            case GLFW_KEY_C: return Key::C;
            case GLFW_KEY_D: return Key::D;
            case GLFW_KEY_E: return Key::E;
            case GLFW_KEY_F: return Key::F;
            case GLFW_KEY_G: return Key::G;
            case GLFW_KEY_H: return Key::H;
            case GLFW_KEY_I: return Key::I;
            case GLFW_KEY_J: return Key::J;
            case GLFW_KEY_K: return Key::K;
            case GLFW_KEY_L: return Key::L;
            case GLFW_KEY_M: return Key::M;
            case GLFW_KEY_N: return Key::N;
            case GLFW_KEY_O: return Key::O;
            case GLFW_KEY_P: return Key::P;
            case GLFW_KEY_Q: return Key::Q;
            case GLFW_KEY_R: return Key::R;
            case GLFW_KEY_S: return Key::S;
            case GLFW_KEY_T: return Key::T;
            case GLFW_KEY_U: return Key::U;
            case GLFW_KEY_V: return Key::V;
            case GLFW_KEY_W: return Key::W;
            case GLFW_KEY_X: return Key::X;
            case GLFW_KEY_Y: return Key::Y;
            case GLFW_KEY_Z: return Key::Z;
            case GLFW_KEY_PERIOD: return Key::Period;
            case GLFW_KEY_COMMA: return Key::Comma;
            case GLFW_KEY_SLASH: return Key::Slash;
            case GLFW_KEY_LEFT_BRACKET: return Key::LeftBracket;
            case GLFW_KEY_RIGHT_BRACKET: return Key::Right;
            case GLFW_KEY_SEMICOLON: return Key::Semicolon;
            case GLFW_KEY_APOSTROPHE: return Key::Apostrophe;
            case GLFW_KEY_BACKSLASH: return Key::Backslash;
            case GLFW_KEY_ENTER: return Key::Enter;
            case GLFW_KEY_DELETE: return Key::Delete;
            case GLFW_KEY_F1: return Key::F1;
            case GLFW_KEY_F2: return Key::F2;
            case GLFW_KEY_F3: return Key::F3;
            case GLFW_KEY_F4: return Key::F4;
            case GLFW_KEY_F5: return Key::F5;
            case GLFW_KEY_F6: return Key::F6;
            case GLFW_KEY_F7: return Key::F7;
            case GLFW_KEY_F8: return Key::F8;
            case GLFW_KEY_F9: return Key::F9;
            case GLFW_KEY_F10: return Key::F10;
            case GLFW_KEY_F11: return Key::F11;
            case GLFW_KEY_F12: return Key::F12;
            case GLFW_KEY_F13: return Key::F13;
            case GLFW_KEY_F14: return Key::F14;
            case GLFW_KEY_F15: return Key::F15;
            case GLFW_KEY_F16: return Key::F16;
            case GLFW_KEY_F17: return Key::F17;
            case GLFW_KEY_F18: return Key::F18;
            case GLFW_KEY_F19: return Key::F19;
            case GLFW_KEY_F20: return Key::F20;
            case GLFW_KEY_F21: return Key::F21;
            case GLFW_KEY_F22: return Key::F22;
            case GLFW_KEY_F23: return Key::F23;
            case GLFW_KEY_F24: return Key::F24;
            case GLFW_KEY_F25: return Key::F25;
            case GLFW_KEY_CAPS_LOCK: return Key::CapsLock;
            case GLFW_KEY_LEFT_SHIFT: return Key::LeftShift;
            case GLFW_KEY_LEFT_CONTROL: return Key::LeftCtrl;
            case GLFW_KEY_LEFT_ALT: return Key::LeftAlt;
            case GLFW_KEY_LEFT_SUPER: return Key::LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT: return Key::RightShift;
            case GLFW_KEY_RIGHT_CONTROL: return Key::RightCtrl;
            case GLFW_KEY_RIGHT_ALT: return Key::RightAlt;
            case GLFW_KEY_RIGHT_SUPER: return Key::RightSuper;
            case GLFW_KEY_UP: return Key::Up;
            case GLFW_KEY_DOWN: return Key::Down;
            case GLFW_KEY_LEFT: return Key::Left;
            case GLFW_KEY_RIGHT: return Key::Right;
            case GLFW_KEY_INSERT: return Key::Insert;
            case GLFW_KEY_MENU: return Key::None;
            case GLFW_KEY_END: return Key::End;
            case GLFW_KEY_HOME: return Key::Home;
            case GLFW_KEY_PAGE_UP: return Key::PageUp;
            case GLFW_KEY_PAGE_DOWN: return Key::PageDown;
            case GLFW_KEY_NUM_LOCK: return Key::NumLock;
            case GLFW_KEY_KP_DIVIDE: return Key::NumpadDiv;
            case GLFW_KEY_KP_MULTIPLY: return Key::NumpadMul;
            case GLFW_KEY_KP_ADD: return Key::NumpadAdd;
            case GLFW_KEY_KP_SUBTRACT: return Key::NumpadSub;
            case GLFW_KEY_KP_1: return Key::Numpad1;
            case GLFW_KEY_KP_2: return Key::Numpad2;
            case GLFW_KEY_KP_3: return Key::Numpad3;
            case GLFW_KEY_KP_4: return Key::Numpad4;
            case GLFW_KEY_KP_5: return Key::Numpad5;
            case GLFW_KEY_KP_6: return Key::Numpad6;
            case GLFW_KEY_KP_7: return Key::Numpad7;
            case GLFW_KEY_KP_8: return Key::Numpad8;
            case GLFW_KEY_KP_9: return Key::Numpad9;
            case GLFW_KEY_KP_0: return Key::Numpad0;
            case GLFW_KEY_KP_DECIMAL: return Key::NumpadPeriod;
            case GLFW_KEY_KP_ENTER: return Key::NumpadEnter;
            case GLFW_KEY_SCROLL_LOCK: return Key::ScrollLock;
            case GLFW_KEY_PRINT_SCREEN: return Key::PrintScreen;
            case GLFW_KEY_PAUSE: return Key::Pause;
            case GLFW_KEY_WORLD_1: return Key::None;
            case GLFW_KEY_WORLD_2: return Key::None;
            default: return Key::None;
        }
    }

    static MouseButton getMouseButtonFromGLFW(int button)
    {
        switch (button)
        {
            case GLFW_MOUSE_BUTTON_LEFT: return MouseButton::Left;
            case GLFW_MOUSE_BUTTON_MIDDLE: return MouseButton::Middle;
            case GLFW_MOUSE_BUTTON_RIGHT: return MouseButton::Right;
            default: return MouseButton::None;
        }
    }

    bool DesktopWindow::initialize(const WindowDesc& windowDesc)
    {
        glfwInit();
        if (m_Handle) glfwDestroyWindow(m_Handle);

#ifdef LUMA_BUILD_OPENGL
        if (windowDesc.deviceType == DeviceType::OpenGL)
        {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        }
#else
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

        if (windowDesc.options & WindowOptions::Centered)
        {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* vidmode = glfwGetVideoMode(primaryMonitor);
            const uint32_t centerX = vidmode->width / 2;
            const uint32_t centerY = vidmode->height / 2;
            const uint32_t x = centerX - windowDesc.width / 2;
            const uint32_t y = centerY - windowDesc.height / 2;

            glfwWindowHint(GLFW_POSITION_X, x);
            glfwWindowHint(GLFW_POSITION_Y, y);
        }

        glfwWindowHint(GLFW_RESIZABLE, windowDesc.options & WindowOptions::Resizable);
        glfwWindowHint(GLFW_DECORATED, !(windowDesc.options & WindowOptions::NoDecoration));

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
                Input::updateKeyState(getKeyFromGLFW(key), InputState::Pressed);

            if (action == GLFW_RELEASE)
                Input::updateKeyState(getKeyFromGLFW(key), InputState::Released);
        });

        glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow*, int button, int action, int)
        {
            if (action == GLFW_PRESS)
                Input::updateMouseButtonState(getMouseButtonFromGLFW(button), InputState::Pressed);
            if (action == GLFW_RELEASE)
                Input::updateMouseButtonState(getMouseButtonFromGLFW(button), InputState::Released);
        });

        glfwSetCursorPosCallback(m_Handle, [](GLFWwindow*, double x, double y)
        {
            Input::updateMousePosition(FVector2d(x, y));
        });

        glfwShowWindow(m_Handle);
        m_Title = windowDesc.title;
        return true;
    }

    void DesktopWindow::destroy()
    {
        if (!m_Handle) return;
        glfwDestroyWindow(m_Handle);
        m_Handle = nullptr;
    }

    void DesktopWindow::pollEvents()
    {
        Input::update();
        glfwPollEvents();
    }

    uint32_t DesktopWindow::getWidth() const
    {
        int32_t width = 0;
        glfwGetWindowSize(m_Handle, &width, nullptr);
        return static_cast<uint32_t>(width);
    }

    uint32_t DesktopWindow::getHeight() const
    {
        int32_t height = 0;
        glfwGetWindowSize(m_Handle, nullptr, &height);
        return static_cast<uint32_t>(height);
    }

    FVector2u DesktopWindow::getPosition() const
    {
        FVector2<int32_t> result;
        glfwGetWindowPos(m_Handle, &result.x, &result.y);
        return result.as<uint32_t>();
    }

    void DesktopWindow::setPosition(const FVector2u& position)
    {
        glfwSetWindowPos(m_Handle, position.x, position.y);
    }

    bool DesktopWindow::hasFocus() const
    {
        return m_Focused;
    }

    bool DesktopWindow::isMaximized() const
    {
        return m_Maximized;
    }

    bool DesktopWindow::isMinimized() const
    {
        return m_Minimized;
    }

    void DesktopWindow::setFullscreen(bool fullscreen)
    {

    }

    bool DesktopWindow::isAvailable() const
    {
        return !isMinimized();
    }

    bool DesktopWindow::shouldClose() const
    {
        return glfwWindowShouldClose(m_Handle);
    }

    GLFWwindow* DesktopWindow::getHandle() const
    {
        return m_Handle;
    }

    String DesktopWindow::getTitle() const
    {
        return m_Title;
    }

    void DesktopWindow::setTitle(const String& title)
    {
        glfwSetWindowTitle(m_Handle, *title);
        m_Title = title;
    }
}

#undef GET_WINDOW