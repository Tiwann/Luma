#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Physics/BoxShape.h"
#include "Luma/Physics/PhysicsBody.h"
#include "Luma/Rendering/Renderer2D.h"

using Luma::Ref;
using Luma::FPhysicsBody;
using Luma::FRenderer2D;
using Luma::FPhysicsWorld;
using Luma::FBoxShape;
using Luma::Camera;


class Player
{
public:
    Player(Ref<FPhysicsWorld> physicsWorld);
    ~Player();

    void update(float deltaTime);
    void render(const Camera& camera, Ref<FRenderer2D> renderer);

    float Speed = 1.0f;
    float JumpHeight = 128.0f;
private:
    static constexpr float PLAYER_SIZE = 64.f;

    Ref<FPhysicsWorld> m_World = nullptr;
    Ref<FPhysicsBody> m_Body = nullptr;
    Ref<FBoxShape> m_BoxShape = nullptr;
};