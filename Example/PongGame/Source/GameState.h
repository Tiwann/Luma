#pragma once
#include "StateMachine.h"

class FGameState : public IState
{
public:
    FGameState(FStateMachine* stateMachine) : IState(stateMachine){}
    void onEnter() override;
    void onUpdate(float deltaTime) override;
    void onExit() override;

    void render();
};
