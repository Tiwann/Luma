#pragma once
#include "StateMachine.h"
#include "MenuState.h"
#include <Luma/Runtime/Application.h>
#include <Luma/Asset/Font.h>

#include "GameState.h"


using Luma::FFont;
using Luma::Ref;
using Luma::WeakRef;
using Luma::IApplication;
using Luma::ICommandBuffer;
using Luma::FApplicationConfig;
using Luma::ERenderDeviceType;


class FGameApplication : public IApplication
{
public:
    FGameApplication(int32_t argc, char** argv);
    FApplicationConfig getConfiguration() const override;
    ERenderDeviceType getRenderDeviceType() const override;

    void onInit() override;
    void onUpdate(float deltaTime) override;
    void onDestroy() override;

    FMenuState* getMenuState() { return &m_MenuState; }
    FGameState* getGameState() { return &m_GameState; }
private:
    FStateMachine m_StateMachine;
    FMenuState m_MenuState;
    FGameState m_GameState;
    Ref<FFont> m_JerseyFont = nullptr;
};
