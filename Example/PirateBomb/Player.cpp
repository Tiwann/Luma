#include "Player.h"

#include "Luma/Physics/BoxShape.h"
#include "Luma/Physics/PhysicsWorld.h"


using namespace Luma;


void Player::init(Ref<FPhysicsWorld> world)
{
    FPhysicsBodyDesc desc{};
    desc.bodyType = EPhysicsBodyType::Dynamic;
    m_Body = world->createBody(desc);

}

void Player::update(float deltaTime)
{

}

void Player::render(Ref<FRenderer2D> renderer, float delta)
{
    const auto position = m_Body->getPosition();
    auto& camera = renderer->getCamera();
    const auto screenSpacePos = camera.worldToScreen(position);

    renderer->drawQuad(FRect2f(0, 0, 100, 100), 0, FColor::White);
}

void Player::destroy()
{
}
