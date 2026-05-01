#pragma once

namespace Luma
{
    enum class EInputDeviceType
    {
        Keyboard,
        Mouse,
        Gamepad
    };

    enum class EKeyCode
    {
        None,
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, 
        Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
        NumpadEnter, NumpadAdd, NumpadSub, NumpadMul, NumpadDiv,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        Escape, Tab, CapsLock,
        LShift, RShift,
        LCtrl,  RCtrl,
        LAlt,   RAlt,
        Space,  Enter, Backspace, Delete,
        Insert, Home,  End,
        PageUp, PageDown,
        Left, Right, Up, Down,
        Grave, Minus, Equals,
        LBracket, RBracket, Backslash,
        Semicolon, Apostrophe, Comma, Period, Slash,
        PrintScreen, ScrollLock, Pause, NumLock,
    };

    enum class EMouseCode
    {
        None,
        Left,
        Right,
        Middle,
        AxisX,
        AxisY,
        DeltaX,
        DeltaY,
        WheelY,
        WheelX,
    };
    
    enum class EGamepadCode
    {
        A, B, X, Y,
        LShoulder, RShoulder,
        LTrigger, RTrigger,
        LThumb, RThumb,
        Start,
        Back,
        Guide,
        DPadUp, DPadDown, DPadLeft, DPadRight,
        LeftStickX, LeftStickY,
        RightStickX, RightStickY,
    };
}