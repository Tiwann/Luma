#include "Luma/Physics/PhysicsWorld.h"
#include "PhysicsWorldInternal.h"
#include "Conversions.h"
#include <box3d/box3d.h>

namespace Luma
{

    PhysicsWorld::PhysicsWorld(const PhysicsWorldDesc& desc)
    {
        m_Pimpl = new Impl();

        if (b3World_IsValid(m_Pimpl->worldId))
            b3DestroyWorld(m_Pimpl->worldId);

        b3WorldDef def = b3DefaultWorldDef();
        def.gravity = convert<b3Vec3>(desc.gravity);
        m_Pimpl->worldId = b3CreateWorld(&def);
        m_Pimpl->timestep = desc.timestep;
        m_Pimpl->substepCount = desc.substepCount;

        LUMA_ASSERT(b3World_IsValid(m_Pimpl->worldId), "Failed to create physics world!");
    }

    void PhysicsWorld::step()
    {
        b3World_Step(m_Pimpl->worldId, m_Pimpl->timestep, m_Pimpl->substepCount);
    }

    void PhysicsWorld::destroy()
    {
        if (b3World_IsValid(m_Pimpl->worldId))
            b3DestroyWorld(m_Pimpl->worldId);
        m_Pimpl->worldId = b3_nullWorldId;
        delete m_Pimpl;
    }

    Ref<PhysicsBody> PhysicsWorld::createBody(const FPhysicsBodyDesc& desc)
    {
        PhysicsBody* body = new PhysicsBody(this, desc);
        return body;
    }

    void PhysicsWorld::destroyBody(Ref<PhysicsBody> body)
    {
        body->destroy();
    }

    void PhysicsWorld::setGravity(const FVector3f& gravity)
    {
        b3World_SetGravity(m_Pimpl->worldId, convert<b3Vec3>(gravity));
    }

    FVector3f PhysicsWorld::getGravity() const
    {
        return convert<FVector3f>(b3World_GetGravity(m_Pimpl->worldId));
    }

    void PhysicsWorld::setTimestep(float timestep)
    {
        m_Pimpl->timestep = timestep;
    }

    float PhysicsWorld::getTimestep()
    {
        return m_Pimpl->timestep;
    }

    PhysicsWorld* createPhysicsWorld(const PhysicsWorldDesc& desc)
    {
        return new PhysicsWorld(desc);
    }
}
