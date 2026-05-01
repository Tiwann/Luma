#pragma once
#include "InputDevice.h"
#include <cstdint>

namespace Luma
{
    enum class EInputEventType
    {
        Pressed,
        Released,
        AxisChanged
    };

    struct FKeyEvent
    {
        EKeyCode code;
        EInputEventType type;
        uint32_t deviceId;
        float value;
        double time;
    };

    struct FMouseEvent
    {
        EMouseCode code;
        EInputEventType type;
        uint32_t deviceId;
        float value;
        double time;
    };

    struct FGamepadEvent
    {
        EGamepadCode code;
        EInputEventType type;
        uint32_t deviceId;
        float value;
        double time;
    };

    struct FInputEvent
    {
        EInputDeviceType deviceType;
        union
        {
            FKeyEvent key;
            FMouseEvent mouse;
            FGamepadEvent gamepad;
        };
    };
}
