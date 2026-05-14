#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Input/Export.h"

union RGFW_event;

namespace Luma
{
    struct IWindow;

    class LUMA_INPUT_API FInput
    {
    public:
        static bool initialize(IWindow* window);
        static void update();
        static void destroy();

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
    private:
        static void onMousePosition(const RGFW_event* e);
        static void onMouseScroll(const RGFW_event* e);
        static constexpr uint32_t MAX_GAMEPADS = 8;
        static inline FKeyboardState m_KeyboardState;
        static inline FMouseState m_MouseState;
        static inline FGamepadState m_GamepadStates[MAX_GAMEPADS];
        static inline IWindow* m_Window = nullptr;
    };
}
