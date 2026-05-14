#pragma once
#include "Luma/Runtime/Flags.h"
#include <cstdint>


namespace Luma
{
    enum class EInputDeviceType
    {
        None,
        Keyboard = BIT(0),
        Mouse = BIT(1),
        Gamepad = BIT(2),
    };

    typedef TFlags<EInputDeviceType> FInputDeviceTypeFlags;

    struct FInputDevice
    {
        uint32_t deviceId = -1u;
        EInputDeviceType type = EInputDeviceType::None;
    };
}
