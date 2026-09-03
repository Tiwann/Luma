#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Input/Export.h"

namespace Luma
{
    class LUMA_INPUT_API FInput
    {
    public:
        static bool getKeyDown(EKey key);
        static bool getKey(EKey key);
        static bool getKeyUp(EKey key);

        static bool getMouseButtonDown(EMouseButton button);
        static bool getMouseButton(EMouseButton button);
        static bool getMouseButtonUp(EMouseButton button);
        static FVector2<double> getMousePosition();
        static FVector2<double> getMouseDelta();
        static double getMouseWheel();

        static bool isGamepadConnected(uint32_t id);
        static bool getGamepadButtonDown(uint32_t id, EGamepadButton button);
        static bool getGamepadButton(uint32_t id, EGamepadButton button);
        static bool getGamepadButtonUp(uint32_t id, EGamepadButton button);
        static double getGamepadAxis(uint32_t id, EGamepadAxis axis);

        static void updateKeyState(EKey key, EInputState state);
        static void updateMouseButtonState(EMouseButton button, EInputState state);
        static void updateMousePosition(const FVector2d& position);
        static void updateMouseWheel(double wheel);
    private:
        static constexpr uint32_t MAX_GAMEPADS = 8;
        static inline FKeyboardState m_KeyboardState;
        static inline FMouseState m_MouseState;
        static inline FGamepadState m_GamepadStates[MAX_GAMEPADS];
    };
}
