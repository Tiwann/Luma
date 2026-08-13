#include "Physics/PhysicsWorld.h"
#include "Conversions.h"
#include <box3d/box3d.h>

namespace Luma
{
    bool FPhysicsWorld::initialize(const FPhysicsWorldDesc& desc)
    {
        if (b3World_IsValid(m_WorldId))
        {
            b3DestroyWorld(m_WorldId);
            // Destroy all physics bodies objects
        }

        b3WorldDef def = b3DefaultWorldDef();
        def.gravity = convert<b3Vec3>(desc.gravity);
        m_WorldId = b3CreateWorld(&def);

        if (!b3World_IsValid(m_WorldId))
            return false;

        return true;
    }

    void FPhysicsWorld::destroy()
    {
        if (b3World_IsValid(m_WorldId))
        {
            b3DestroyWorld(m_WorldId);
            // Destroy all physics bodies objects
        }
    }

    void FPhysicsWorld::step(const float timeStep, const uint32_t substepCount)
    {
        b3World_Step(m_WorldId, timeStep, substepCount);
    }

    FPhysicsBody* FPhysicsWorld::createBody(const FPhysicsBodyDesc& desc)
    {
        FPhysicsBody* body = new FPhysicsBody();
        FPhysicsBodyDesc bodyDesc = desc;
        bodyDesc.world = this;

        if (!body->initialize(bodyDesc))
        {
            delete body;
            return nullptr;
        }

        m_Bodies.add(body);
        return body;
    }

    void FPhysicsWorld::destroyBody(FPhysicsBody* body)
    {
        body->destroy();
        // remove from array
    }

    void FPhysicsWorld::setGravity(const FVector3f& gravity)
    {
        b3World_SetGravity(m_WorldId, convert<b3Vec3>(gravity));
    }

    FVector3f FPhysicsWorld::getGravity() const
    {
        return convert<FVector3f>(b3World_GetGravity(m_WorldId));
    }
}
