#include "Luma/Input/Immediate.h"

namespace Luma
{
    bool FInput::getKeyDown(EKey key)
    {
        return m_KeyboardState.currentState[(uint32_t)key] == EInputState::Pressed &&
            m_KeyboardState.previousState[(uint32_t)key] != EInputState::Pressed;
    }

    bool FInput::getKey(EKey key)
    {
        return m_KeyboardState.currentState[(uint32_t)key] == EInputState::Pressed;
    }

    bool FInput::getKeyUp(EKey key)
    {
        return m_KeyboardState.currentState[(uint32_t)key] == EInputState::Released &&
            m_KeyboardState.previousState[(uint32_t)key] != EInputState::Released;
    }

    bool FInput::getMouseButtonDown(EMouseButton button)
    {
        return m_MouseState.currentButtons[(uint32_t)button] == EInputState::Pressed &&
            m_MouseState.previousButtons[(uint32_t)button] != EInputState::Pressed;
    }

    bool FInput::getMouseButton(EMouseButton button)
    {
        return m_MouseState.currentButtons[(uint32_t)button] == EInputState::Pressed;
    }

    bool FInput::getMouseButtonUp(EMouseButton button)
    {
        return m_MouseState.currentButtons[(uint32_t)button] == EInputState::Released &&
            m_MouseState.previousButtons[(uint32_t)button] != EInputState::Released;
    }

    FVector2<double> FInput::getMousePosition()
    {
        return m_MouseState.currentPosition;
    }

    FVector2<double> FInput::getMouseDelta()
    {
        return m_MouseState.currentPosition - m_MouseState.previousPosition;
    }

    double FInput::getMouseWheel()
    {
        return m_MouseState.currentWheel;
    }

    bool FInput::isGamepadConnected(const uint32_t id)
    {
        return m_GamepadStates[id].connected;
    }

    bool FInput::getGamepadButtonDown(const uint32_t id, EGamepadButton button)
    {
        return m_GamepadStates[id].currentButtons[(uint32_t)button] == EInputState::Pressed &&
            m_GamepadStates[id].previousButtons[(uint32_t)button] != EInputState::Pressed;
    }

    bool FInput::getGamepadButton(const uint32_t id, EGamepadButton button)
    {
        return m_GamepadStates[id].currentButtons[(uint32_t)button] == EInputState::Pressed;
    }

    bool FInput::getGamepadButtonUp(const uint32_t id, EGamepadButton button)
    {
        return m_GamepadStates[id].currentButtons[(uint32_t)button] == EInputState::Released &&
            m_GamepadStates[id].previousButtons[(uint32_t)button] != EInputState::Released;
    }

    double FInput::getGamepadAxis(const uint32_t id, EGamepadAxis axis)
    {
        return m_GamepadStates[id].currentAxes[(uint32_t)axis];
    }

    void FInput::updateKeyState(EKey key, EInputState state)
    {
        m_KeyboardState.previousState[(uint32_t)key] = m_KeyboardState.currentState[(uint32_t)key];
        m_KeyboardState.currentState[(uint32_t)key] = state;
    }

    void FInput::updateMouseButtonState(EMouseButton button, EInputState state)
    {
        m_MouseState.previousButtons[(uint32_t)button] = m_MouseState.currentButtons[(uint32_t)button];
        m_MouseState.currentButtons[(uint32_t)button] = state;
    }

    void FInput::updateMousePosition(FVector2d position)
    {
        m_MouseState.previousPosition = m_MouseState.currentPosition;
        m_MouseState.currentPosition = position;
    }

    void FInput::updateMouseWheel(double wheel)
    {
        m_MouseState.previousWheel = m_MouseState.currentWheel;
        m_MouseState.currentWheel = wheel;
    }
}
