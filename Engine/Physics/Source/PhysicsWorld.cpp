#include "Luma/Physics/PhysicsWorld.h"
#include "PhysicsWorldInternal.h"
#include "Conversions.h"
#include <box3d/box3d.h>

namespace Luma
{

    FPhysicsWorld::FPhysicsWorld(const FPhysicsWorldDesc& desc)
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

    void FPhysicsWorld::step()
    {
        b3World_Step(m_Pimpl->worldId, m_Pimpl->timestep, m_Pimpl->substepCount);
    }

    void FPhysicsWorld::destroy()
    {
        if (b3World_IsValid(m_Pimpl->worldId))
            b3DestroyWorld(m_Pimpl->worldId);
        m_Pimpl->worldId = b3_nullWorldId;
        delete m_Pimpl;
    }

    Ref<FPhysicsBody> FPhysicsWorld::createBody(const FPhysicsBodyDesc& desc)
    {
        FPhysicsBody* body = new FPhysicsBody(this, desc);
        return body;
    }

    void FPhysicsWorld::destroyBody(Ref<FPhysicsBody> body)
    {
        body->destroy();
    }

    void FPhysicsWorld::setGravity(const FVector3f& gravity)
    {
        b3World_SetGravity(m_Pimpl->worldId, convert<b3Vec3>(gravity));
    }

    FVector3f FPhysicsWorld::getGravity() const
    {
        return convert<FVector3f>(b3World_GetGravity(m_Pimpl->worldId));
    }

    void FPhysicsWorld::setTimestep(float timestep)
    {
        m_Pimpl->timestep = timestep;
    }

    float FPhysicsWorld::getTimestep()
    {
        return m_Pimpl->timestep;
    }
}
