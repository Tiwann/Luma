#include "Player.h"

#include "Luma/Input/Immediate.h"
#include "Luma/Physics/BoxShape.h"
#include "Luma/Physics/PhysicsWorld.h"

using namespace Luma;


Player::Player(Ref<FPhysicsWorld> physicsWorld)
    : m_World(physicsWorld)
{
    FPhysicsBodyDesc desc{};
    desc.bodyType = EPhysicsBodyType::Dynamic;
    m_Body = m_World->createBody(desc);
    m_Body->setConstraints(EPhysicsConstraints::PositionZ);

    m_BoxShape = Ref<FBoxShape>::create(FVector3f(PLAYER_SIZE * 0.5f));
    m_Body->attachShape(m_BoxShape);
}

Player::~Player()
{
    m_World->destroyBody(m_Body);
    m_Body = nullptr;
    m_World = nullptr;
}

void Player::update(float deltaTime)
{
    if (FInput::getKeyDown(EKey::Space))
    {
        const auto mass = m_Body->getMass();
        const auto gravity = m_World->getGravity();
        const auto jumpVelocity = std::sqrt(-2.0 * gravity.y * JumpHeight);
        const auto force = FVector3f::Up * (mass * jumpVelocity / deltaTime);
        m_Body->addImpulse(force);
    }
}


void Player::render(const FCamera& camera, Ref<FRenderer2D> renderer)
{
    const auto position = m_Body->getPosition();
    renderer->drawQuad(FVector2f(position), FVector2f(PLAYER_SIZE), 0, FColor::White);
}