#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Physics/PhysicsBody.h"
#include "Luma/Rendering/Renderer2D.h"

using Luma::Ref;
using Luma::FPhysicsBody;
using Luma::FRenderer2D;
using Luma::FPhysicsWorld;

class Player
{
public:
    void init(Ref<FPhysicsWorld> world);
    void update(float deltaTime);
    void render(Ref<FRenderer2D> renderer, float delta);
    void destroy();

private:
    Ref<FPhysicsBody> m_Body = nullptr;
    float m_Speed = 1.0f;

};