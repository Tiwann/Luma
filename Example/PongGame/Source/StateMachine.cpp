#include "StateMachine.h"
#include "Luma/Runtime/Assertion.h"

void FStateMachine::changeState(IState* newState)
{
    m_PendingState = newState;
}

void FStateMachine::initialize(IState* entryPoint)
{
    LUMA_ASSERT(entryPoint, "Must initialize state machine with a valid state entry point!");
    if (!entryPoint) return;
    m_CurrentState = entryPoint;
    m_CurrentState->onEnter();
}

void FStateMachine::update(const float deltaTime)
{
    if (m_PendingState)
    {
        m_CurrentState->onExit();
        m_CurrentState = m_PendingState;
        m_CurrentState->onEnter();

        m_PreviousState = m_PendingState;
        m_PendingState = nullptr;
        return;
    }

    m_CurrentState->onUpdate(deltaTime);
}

void FStateMachine::destroy()
{
    m_CurrentState->onExit();
    m_CurrentState = nullptr;
    m_PendingState = nullptr;
    m_PreviousState = nullptr;
}

IState* FStateMachine::getCurrentState() const
{
    return m_CurrentState;
}

IState* FStateMachine::getPendingState() const
{
    return m_PendingState;
}

IState* FStateMachine::getPreviousState() const
{
    return m_PreviousState;
}

void* FStateMachine::getUserData() const
{
    return m_UserData;
}

void* FStateMachine::setUserData(void* userData)
{
    void* oldUserData = m_UserData;
    m_UserData = userData;
    return oldUserData;
}
