#include "Luma/Input/Immediate.h"
#include "Luma/Memory/Memory.h"
#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Runtime/Window.h"
#include <rgfw/rgfw.h>

namespace Luma
{

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

    bool FInput::initialize(IWindow* window)
    {
        if (!window) return false;
        Memory::memset(&m_KeyboardState, 0, sizeof(FKeyboardState));
        Memory::memset(&m_MouseState, 0, sizeof(FMouseState));
        Memory::memset(&m_GamepadStates, 0, MAX_GAMEPADS * sizeof(FGamepadState));
        m_Window = window;
        return true;
    }

    void FInput::onMousePosition(const RGFW_event* e)
    {
        m_MouseState.previousPosition = m_MouseState.currentPosition;
        m_MouseState.currentPosition = {static_cast<double>(e->mouse.x), static_cast<double>(e->mouse.y)};
    };

    void FInput::onMouseScroll(const RGFW_event* e)
    {
        m_MouseState.previousWheel = m_MouseState.currentWheel;
        m_MouseState.currentWheel = {static_cast<double>(e->delta.y)};
    };

    void FInput::update()
    {
        if (!m_Window) return;

        if (FDesktopWindow* windowImpl = dynamic_cast<FDesktopWindow*>(m_Window))
        {
            RGFW_window* handle = windowImpl->getHandle();
            const RGFW_genericFunc prevMousePosCallback = RGFW_setEventCallback(RGFW_mousePosChanged, onMousePosition);
            const RGFW_genericFunc prevMouseScrollCallback = RGFW_setEventCallback(RGFW_mouseScroll, onMouseScroll);

            windowImpl->pollEvents();

            Memory::memcpy(&m_KeyboardState.previousState, &m_KeyboardState.currentState, sizeof(EInputState) * MAX_KEYBOARD_KEYS);
            for (uint32_t key = 0; key < RGFW_keyLast; ++key)
            {
                const EInputState keyState =
                    RGFW_window_isKeyDown(handle, key) ? EInputState::Pressed : EInputState::Released;
                m_KeyboardState.currentState[(uint32_t)getKeyFromRGFW(key)] = keyState;
            }

            Memory::memcpy(&m_MouseState.previousButtons, &m_MouseState.currentButtons, sizeof(EInputState) * MAX_MOUSE_BUTTONS);
            for (uint32_t mouseButton = 0; mouseButton < RGFW_mouseFinal; ++mouseButton)
            {
                const EInputState mouseButtonState =
                    RGFW_window_isMouseDown(handle, mouseButton) ? EInputState::Pressed : EInputState::Released;
                m_MouseState.currentButtons[(uint32_t)getMouseButtonFromRGFW(mouseButton)] = mouseButtonState;
            }


            RGFW_setEventCallback(RGFW_mousePosChanged, prevMousePosCallback);
            RGFW_setEventCallback(RGFW_mouseScroll, prevMouseScrollCallback);
        }
    }

    void FInput::destroy()
    {
        Memory::memset(&m_KeyboardState, 0, sizeof(FKeyboardState));
        Memory::memset(&m_MouseState, 0, sizeof(FMouseState));
        Memory::memset(&m_GamepadStates, 0, MAX_GAMEPADS * sizeof(FGamepadState));
    }

    bool FInput::getKeyDown(EKey key)
    {
        return m_KeyboardState.currentState[(uint32_t)key] == EInputState::Pressed &&
            m_KeyboardState.previousState[(uint32_t)key] != EInputState::Pressed;
    }

    bool FInput::getKey(EKey key)
    {
        return m_KeyboardState.currentState[(uint32_t)key] == EInputState::Pressed;
    }

    bool FInput::getKeyUp(EKey key)
    {
        return m_KeyboardState.currentState[(uint32_t)key] == EInputState::Released &&
            m_KeyboardState.previousState[(uint32_t)key] != EInputState::Released;
    }

    bool FInput::getMouseButtonDown(EMouseButton button)
    {
        return m_MouseState.currentButtons[(uint32_t)button] == EInputState::Pressed &&
            m_MouseState.previousButtons[(uint32_t)button] != EInputState::Pressed;
    }

    bool FInput::getMouseButton(EMouseButton button)
    {
        return m_MouseState.currentButtons[(uint32_t)button] == EInputState::Pressed;
    }

    bool FInput::getMouseButtonUp(EMouseButton button)
    {
        return m_MouseState.currentButtons[(uint32_t)button] == EInputState::Released &&
            m_MouseState.previousButtons[(uint32_t)button] != EInputState::Released;
    }

    FVector2<double> FInput::getMousePosition()
    {
        return m_MouseState.currentPosition;
    }

    FVector2<double> FInput::getMouseDelta()
    {
        return m_MouseState.currentPosition - m_MouseState.previousPosition;
    }

    double FInput::getMouseWheel()
    {
        return m_MouseState.currentWheel;
    }

    bool FInput::isGamepadConnected(const uint32_t id)
    {
        return m_GamepadStates[id].connected;
    }

    bool FInput::getGamepadButtonDown(const uint32_t id, EGamepadButton button)
    {
        return m_GamepadStates[id].currentButtons[(uint32_t)button] == EInputState::Pressed &&
            m_GamepadStates[id].previousButtons[(uint32_t)button] != EInputState::Pressed;
    }

    bool FInput::getGamepadButton(const uint32_t id, EGamepadButton button)
    {
        return m_GamepadStates[id].currentButtons[(uint32_t)button] == EInputState::Pressed;
    }

    bool FInput::getGamepadButtonUp(const uint32_t id, EGamepadButton button)
    {
        return m_GamepadStates[id].currentButtons[(uint32_t)button] == EInputState::Released &&
            m_GamepadStates[id].previousButtons[(uint32_t)button] != EInputState::Released;
    }

    double FInput::getGamepadAxis(const uint32_t id, EGamepadAxis axis)
    {
        return m_GamepadStates[id].currentAxes[(uint32_t)axis];
    }
}
