#include "Game.h"
#include <Luma/Runtime/Path.h>
#include <Luma/Rendering/Renderer2D.h>

#include "Luma/Runtime/Ease.h"

using namespace Luma;

static const FString TITLE = "PONG GAME";
static const float TITLE_SIZE = 150;
static constexpr uint32_t WIDTH = 1280;
static constexpr uint32_t HEIGHT = 720;

FGame::FGame(int32_t argc, char** argv): IApplication(argc, argv)
{}

FApplicationConfiguration FGame::getConfiguration() const
{
    FApplicationConfiguration configuration;
    configuration.applicationName = TITLE;
    configuration.windowWidth = WIDTH;
    configuration.windowHeight = HEIGHT;
    configuration.windowFlags = EWindowCreateBits::Centered;
    configuration.vsync = true;
    return configuration;
}

ERenderDeviceType FGame::getRenderDeviceType() const
{
    return ERenderDeviceType::Vulkan;
}

void FGame::onInit()
{
    const auto& asciiCharSet = FCharacterSet::ascii();
    const auto jerseyFontFilepath = FPath::getAssetPath("Fonts/Jersey10-Regular.ttf");
    m_JerseyFont = Ref<FFont>::create();
    m_JerseyFont->loadAndGenerate(jerseyFontFilepath, EFontAtlasType::MSDF, {asciiCharSet}, getRenderDevice());
    WeakRef<FRenderer2D> renderer = getRenderer2D();
    renderer->setFont(m_JerseyFont);
}

static void drawTextCentered(WeakRef<FRenderer2D> renderer, WeakRef<FFont> font, const FRect2<float> bounds, const FStringView text, const float fontSize)
{
    const auto width = font->getTextWidth(text, fontSize);
    const auto height = font->getTextHeight(text, fontSize);
    const float x = bounds.width * 0.5f - width * 0.5f;
    const float y = bounds.height * 0.5f - height;
    renderer->drawText(text, {x, y}, fontSize, FColor::White);
}

void FGame::onUpdate(const float deltaTime)
{
    const WeakRef<FRenderer2D> renderer = getRenderer2D();

    static bool animate = true;
    static float time = 0.0f;
    if (animate && time <= 1.0f)
        time += deltaTime;

    constexpr auto exp = getEaseFunction<float>(EEaseType::ExpoOut);
    const float fromSize = TITLE_SIZE * 0.4f;
    const float toSize = TITLE_SIZE;
    const float finalSize = std::lerp<float>(fromSize, toSize, std::clamp(exp(time), 0.0f, 1.0f));

    drawTextCentered(renderer, m_JerseyFont, {0, 0, WIDTH, HEIGHT}, TITLE, finalSize);
}

void FGame::onDestroy()
{
    m_JerseyFont->destroy();
}
