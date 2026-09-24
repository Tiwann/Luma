#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Input/Export.h"

namespace Luma
{
    class LUMA_INPUT_API Input
    {
    public:
        static bool getKeyDown(Key key);
        static bool getKey(Key key);
        static bool getKeyUp(Key key);

        static bool getMouseButtonDown(MouseButton button);
        static bool getMouseButton(MouseButton button);
        static bool getMouseButtonUp(MouseButton button);
        static FVector2<double> getMousePosition();
        static FVector2<double> getMouseDelta();
        static double getMouseWheel();

        static bool isGamepadConnected(uint32_t id);
        static bool getGamepadButtonDown(uint32_t id, GamepadButton button);
        static bool getGamepadButton(uint32_t id, GamepadButton button);
        static bool getGamepadButtonUp(uint32_t id, GamepadButton button);
        static double getGamepadAxis(uint32_t id, GamepadAxis axis);

        static void updateKeyState(Key key, InputState state);
        static void updateMouseButtonState(MouseButton button, InputState state);
        static void updateMousePosition(const FVector2d& position);
        static void updateMouseWheel(double wheel);
        static void update();
    private:
        static constexpr uint32_t MAX_GAMEPADS = 8;
        static inline KeyboardState m_KeyboardState;
        static inline MouseState m_MouseState;
        static inline GamepadState m_GamepadStates[MAX_GAMEPADS];
    };
}
