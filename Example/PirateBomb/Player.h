#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Physics/BoxShape.h"
#include "Luma/Physics/PhysicsBody.h"
#include "Luma/Rendering/Renderer2D.h"

using Luma::Ref;
using Luma::PhysicsBody;
using Luma::Renderer2D;
using Luma::PhysicsWorld;
using Luma::BoxShape;
using Luma::Camera;


class Player
{
public:
    Player(Ref<PhysicsWorld> physicsWorld);
    ~Player();

    void update(float deltaTime);
    void render(const Camera& camera, Ref<Renderer2D> renderer);

    float Speed = 1.0f;
    float JumpHeight = 128.0f;
private:
    static constexpr float PLAYER_SIZE = 64.f;

    Ref<PhysicsWorld> m_World = nullptr;
    Ref<PhysicsBody> m_Body = nullptr;
    Ref<BoxShape> m_BoxShape = nullptr;
};