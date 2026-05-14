#pragma once
#include "StateMachine.h"

class FMenuState : public IState
{
public:
    FMenuState(FStateMachine* stateMachine): IState(stateMachine){}
    void onUpdate(float deltaTime) override;
};
