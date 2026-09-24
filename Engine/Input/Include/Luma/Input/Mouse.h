#pragma once
#include "Luma/Input/InputState.h"
#include "Luma/Math/Vector2.h"
#include <cstdint>
#include <array>

namespace Luma
{
    enum class MouseButton
    {
        None,
        Left,
        Right,
        Middle,
        _ButtonCount
    };

    static constexpr uint32_t MAX_MOUSE_BUTTONS = static_cast<uint32_t>(MouseButton::_ButtonCount);

    struct MouseState
    {
        std::array<InputState, MAX_MOUSE_BUTTONS> currentButtons;
        std::array<InputState, MAX_MOUSE_BUTTONS> previousButtons;
        double currentWheel = 0.0;
        double previousWheel = 0.0;
        FVector2<double> currentPosition{};
        FVector2<double> previousPosition{};
    };
}
