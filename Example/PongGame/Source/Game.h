#pragma once
#include <Luma/Runtime/Application.h>
#include <Luma/Asset/Font.h>

using Luma::FFont;
using Luma::Ref;
using Luma::WeakRef;
using Luma::IApplication;
using Luma::ICommandBuffer;
using Luma::FApplicationConfiguration;
using Luma::ERenderDeviceType;

enum class EGameState
{
    Menu,
    Game,
    Score
};

class FGame : public IApplication
{
public:
    FGame(int32_t argc, char** argv);
    FApplicationConfiguration getConfiguration() const override;
    ERenderDeviceType getRenderDeviceType() const override;

    void onInit() override;
    void onUpdate(float deltaTime) override;
    void onDestroy() override;

private:
    EGameState m_GameState = EGameState::Menu;
    Ref<FFont> m_JerseyFont = nullptr;
};
