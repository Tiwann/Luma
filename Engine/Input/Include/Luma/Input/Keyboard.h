#pragma once
#include "InputState.h"
#include <cstdint>

#include "Luma/Memory/Memory.h"

namespace Luma
{
    enum class EKey
    {
        None,
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4,
        Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
        NumpadEnter, NumpadAdd, NumpadSub, NumpadMul, NumpadDiv, NumpadPeriod,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14,
        F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
        Escape, Tab, CapsLock,
        LeftShift, RightShift,
        LeftCtrl,  RightCtrl,
        LeftAlt,   RightAlt,
        Space,  Enter, Backspace, Delete,
        Insert, Home,  End,
        PageUp, PageDown,
        Left, Right, Up, Down,
        Grave, Minus, Equals,
        LeftBracket, RightBracket, Backslash,
        Semicolon, Apostrophe, Comma, Period, Slash,
        PrintScreen, ScrollLock, Pause, NumLock,
        LeftSuper, RightSuper,
        _KeyCount
    };

    static constexpr uint32_t MAX_KEYBOARD_KEYS = static_cast<uint32_t>(EKey::_KeyCount);

    struct FKeyboardState
    {
        EInputState currentState[MAX_KEYBOARD_KEYS]{};
        EInputState previousState[MAX_KEYBOARD_KEYS]{};
    };
}
