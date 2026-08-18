#include "GameApplication.h"
#include <Luma/Runtime/Path.h>
#include <Luma/Rendering/Renderer2D.h>
#include <Luma/Input/Immediate.h>
#include <Luma/Math/Math.h>
#include <Luma/Runtime/Time.h>

using namespace Luma;

FGameApplication::FGameApplication(const int argc, char** argv)
    : IApplication(argc, argv), m_MenuState(&m_StateMachine), m_GameState(&m_StateMachine)
{}

FApplicationConfig FGameApplication::getConfiguration() const
{
    FApplicationConfig configuration;
    configuration.applicationName = "PONG GAME";
    configuration.windowWidth = 1280;
    configuration.windowHeight = 720;
    configuration.windowFlags = EWindowCreateBits::Centered;
    configuration.vsync = true;
    return configuration;
}

ERenderDeviceType FGameApplication::getRenderDeviceType() const
{
    return ERenderDeviceType::Vulkan;
}

void FGameApplication::onInit()
{
    const auto& asciiCharSet = FCharacterSet::ascii();
    const auto jerseyFontFilepath = FPath::getAssetPath("Fonts/Jersey10-Regular.ttf");
    m_JerseyFont = Ref<FFont>::create();
    m_JerseyFont->loadAndGenerate(jerseyFontFilepath, EFontAtlasType::MSDF, {asciiCharSet}, getRenderDevice());
    WeakRef<FRenderer2D> renderer = getRenderer2D();
    renderer->setFont(m_JerseyFont);

    m_StateMachine.initialize(&m_MenuState);
    m_StateMachine.setUserData(this);
}


void FGameApplication::onUpdate(const float deltaTime)
{
    m_StateMachine.update(deltaTime);
}

void FGameApplication::onDestroy()
{
    m_StateMachine.destroy();
    m_JerseyFont->destroy();
}