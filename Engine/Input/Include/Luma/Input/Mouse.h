#pragma once
#include "Luma/Input/InputState.h"
#include "Luma/Math/Vector2.h"
#include <cstdint>
#include <array>

namespace Luma
{
    enum class EMouseButton
    {
        None,
        Left,
        Right,
        Middle,
        _ButtonCount
    };

    static constexpr uint32_t MAX_MOUSE_BUTTONS = static_cast<uint32_t>(EMouseButton::_ButtonCount);

    struct FMouseState
    {
        std::array<EInputState, MAX_MOUSE_BUTTONS> currentButtons;
        std::array<EInputState, MAX_MOUSE_BUTTONS> previousButtons;
        double currentWheel = 0.0;
        double previousWheel = 0.0;
        FVector2<double> currentPosition{};
        FVector2<double> previousPosition{};
    };
}
