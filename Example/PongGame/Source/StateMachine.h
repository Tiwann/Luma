#pragma once

class FStateMachine;

struct IState
{
    IState(FStateMachine* stateMachine) : m_StateMachine(stateMachine) {}
    virtual ~IState() = default;
    virtual void onEnter(){}
    virtual void onUpdate(float deltaTime){}
    virtual void onExit(){}
protected:
    FStateMachine* m_StateMachine = nullptr;
};

class FStateMachine
{
public:
    void changeState(IState* newState);
    void initialize(IState* entryPoint);
    void update(float deltaTime);
    void destroy();

    IState* getCurrentState() const;
    IState* getPendingState() const;
    IState* getPreviousState() const;

    void* getUserData() const;
    void* setUserData(void* userData);
private:
    void* m_UserData = nullptr;
    IState* m_PreviousState = nullptr;
    IState* m_CurrentState = nullptr;
    IState* m_PendingState = nullptr;
};