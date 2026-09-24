#include "Luma/Input/Immediate.h"

namespace Luma
{
    bool Input::getKeyDown(Key key)
    {
           return m_KeyboardState.currentState[(uint32_t)key] == InputState::Pressed &&
            m_KeyboardState.previousState[(uint32_t)key] != InputState::Pressed;
    }

    bool Input::getKey(Key key)
    {
        return m_KeyboardState.currentState[(uint32_t)key] == InputState::Pressed;
    }

    bool Input::getKeyUp(Key key)
    {
        return m_KeyboardState.currentState[(uint32_t)key] == InputState::Released &&
            m_KeyboardState.previousState[(uint32_t)key] != InputState::Released;
    }

    bool Input::getMouseButtonDown(MouseButton button)
    {
        return m_MouseState.currentButtons[(uint32_t)button] == InputState::Pressed &&
            m_MouseState.previousButtons[(uint32_t)button] != InputState::Pressed;
    }

    bool Input::getMouseButton(MouseButton button)
    {
        return m_MouseState.currentButtons[(uint32_t)button] == InputState::Pressed;
    }

    bool Input::getMouseButtonUp(MouseButton button)
    {
        return m_MouseState.currentButtons[(uint32_t)button] == InputState::Released &&
            m_MouseState.previousButtons[(uint32_t)button] != InputState::Released;
    }

    FVector2<double> Input::getMousePosition()
    {
        return m_MouseState.currentPosition;
    }

    FVector2<double> Input::getMouseDelta()
    {
        return m_MouseState.currentPosition - m_MouseState.previousPosition;
    }

    double Input::getMouseWheel()
    {
        return m_MouseState.currentWheel;
    }

    bool Input::isGamepadConnected(const uint32_t id)
    {
        return m_GamepadStates[id].connected;
    }

    bool Input::getGamepadButtonDown(const uint32_t id, GamepadButton button)
    {
        return m_GamepadStates[id].currentButtons[(uint32_t)button] == InputState::Pressed &&
            m_GamepadStates[id].previousButtons[(uint32_t)button] != InputState::Pressed;
    }

    bool Input::getGamepadButton(const uint32_t id, GamepadButton button)
    {
        return m_GamepadStates[id].currentButtons[(uint32_t)button] == InputState::Pressed;
    }

    bool Input::getGamepadButtonUp(const uint32_t id, GamepadButton button)
    {
        return m_GamepadStates[id].currentButtons[(uint32_t)button] == InputState::Released &&
            m_GamepadStates[id].previousButtons[(uint32_t)button] != InputState::Released;
    }

    double Input::getGamepadAxis(const uint32_t id, GamepadAxis axis)
    {
        return m_GamepadStates[id].currentAxes[(uint32_t)axis];
    }

    void Input::updateKeyState(Key key, InputState state)
    {
        m_KeyboardState.currentState[(uint32_t)key] = state;
    }

    void Input::updateMouseButtonState(MouseButton button, InputState state)
    {
        m_MouseState.currentButtons[(uint32_t)button] = state;
    }

    void Input::updateMousePosition(const FVector2d& position)
    {
        m_MouseState.currentPosition = position;
    }

    void Input::updateMouseWheel(double wheel)
    {
        m_MouseState.currentWheel = wheel;
    }

    void Input::update()
    {
        m_KeyboardState.previousState = m_KeyboardState.currentState;
        m_MouseState.previousButtons = m_MouseState.currentButtons;
        m_MouseState.previousPosition = m_MouseState.currentPosition;
        m_MouseState.previousWheel = m_MouseState.currentWheel;
    }
}
