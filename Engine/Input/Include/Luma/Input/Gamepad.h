#pragma once
#include "InputState.h"
#include <cstdint>

namespace Luma
{
    enum class EGamepadButton
    {
        North, South, East, West,
        LeftShoulder, RightShoulder,
        LeftTrigger, RightTrigger,
        LeftThumb, RightThumb,
        Start,
        Back,
        Guide,
        DPadUp, DPadDown, DPadLeft, DPadRight,
        _ButtonCount
    };

    static constexpr uint32_t MAX_GAMEPAD_BUTTONS = static_cast<uint32_t>(EGamepadButton::_ButtonCount);

    enum class EGamepadAxis
    {
        LeftX, LeftY,
        RightX, RightY,
        LeftTrigger,
        RightTrigger,
        _AxisCount
    };

    static constexpr uint32_t MAX_GAMEPAD_AXIS = static_cast<uint32_t>(EGamepadAxis::_AxisCount);

    struct FGamepadState
    {
        EInputState currentButtons[MAX_GAMEPAD_BUTTONS];
        EInputState previousButtons[MAX_GAMEPAD_BUTTONS];
        double currentAxes[MAX_GAMEPAD_AXIS];
        double previousAxes[MAX_GAMEPAD_AXIS];
        uint32_t connected;
    };
}