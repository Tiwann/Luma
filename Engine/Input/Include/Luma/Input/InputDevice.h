#pragma once
#include "Luma/Runtime/Flags.h"
#include <cstdint>


namespace Luma
{
    enum class InputDeviceType
    {
        None,
        Keyboard = BIT(0),
        Mouse = BIT(1),
        Gamepad = BIT(2),
    };

    typedef TFlags<InputDeviceType> InputDeviceFlags;

    struct InputDevice
    {
        uint32_t deviceId = -1u;
        InputDeviceType type = InputDeviceType::None;
    };
}
