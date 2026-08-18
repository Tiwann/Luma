#include "MenuState.h"
#include "GameApplication.h"
#include "Luma/Input/Immediate.h"
#include "Luma/Math/Math.h"

using namespace Luma;

void FMenuState::onUpdate(const float deltaTime)
{
    FGameApplication* application = static_cast<FGameApplication*>(m_StateMachine->getUserData());
    if (!application) return;

    const auto renderer = application->getRenderer2D();

    static float time = 0.0f;
    time += deltaTime;

    const auto normalizeSegment = [](float start, float duration) {
        return FMath::saturate((time - start) / duration);
    };

    const auto titleAnimTime = normalizeSegment(0.0f, 0.75f);
    const auto bounds = application->getWindow()->getBounds().as<float>();
    renderer->drawTextCentered("PONG GAME", bounds.center(), titleAnimTime * 150, FColor::White);
    if (time >=0.75f)
    {
        const auto position = bounds.center() + FVector2f::Up * 50;
        const auto opacity = (FMath::cos(5.0f * time) + 1.0f) / 2.0f;
        const auto color = FColor::White.withOpacity(opacity);
        renderer->drawTextCentered("Press space to play", position, 50, color);
    }

    if (FInput::getKeyDown(EKey::Space))
        m_StateMachine->changeState(application->getGameState());
}
