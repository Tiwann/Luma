#include "Physics/PhysicsBody.h"
#include "Physics/PhysicsWorld.h"
#include "Conversions.h"
#include <box3d/box3d.h>

namespace Luma
{
    bool FPhysicsBody::initialize(const FPhysicsBodyDesc& desc)
    {
        if (!desc.world) return false;

        if (b3Body_IsValid(m_BodyId))
            b3DestroyBody(m_BodyId);

        b3BodyDef def = b3DefaultBodyDef();
        def.userData = this;
        def.type = convert<b3BodyType>(desc.bodyType);
        def.position = convert<b3Vec3>(desc.initialPosition);
        def.rotation = convert<b3Quat>(desc.initialRotation);
        def.linearVelocity = convert<b3Vec3>(desc.initialLinearVelocity);
        def.angularVelocity = convert<b3Vec3>(desc.initialAngularVelocity);
        def.linearDamping = desc.linearDamping;
        def.angularDamping = desc.angularDamping;
        def.motionLocks = convert<b3MotionLocks>(desc.constraints);

        m_BodyId = b3CreateBody(desc.world->getWorldId(), &def);

        if (!b3Body_IsValid(m_BodyId))
            return false;

        m_BodyType = desc.bodyType;
        return true;
    }

    void FPhysicsBody::destroy()
    {
        b3DestroyBody(m_BodyId);
        m_BodyId = b3_nullBodyId;
    }

    void FPhysicsBody::setBodyType(EPhysicsBodyType bodyType)
    {
        b3Body_SetType(m_BodyId, convert<b3BodyType>(bodyType));
    }

    EPhysicsBodyType FPhysicsBody::getBodyType() const
    {
        return convert<EPhysicsBodyType>(b3Body_GetType(m_BodyId));
    }

    void FPhysicsBody::setPosition(const FVector3f& position)
    {
        b3Body_SetTransform(m_BodyId, convert<b3Vec3>(position), b3Body_GetRotation(m_BodyId));
    }

    FVector3f FPhysicsBody::getPosition() const
    {
        return convert<FVector3f>(b3Body_GetPosition(m_BodyId));
    }

    void FPhysicsBody::setRotation(const FQuatf& rotation)
    {
        b3Body_SetTransform(m_BodyId, b3Body_GetPosition(m_BodyId), convert<b3Quat>(rotation));
    }

    FQuatf FPhysicsBody::getRotation()
    {
        return convert<FQuatf>(b3Body_GetRotation(m_BodyId));
    }

    void FPhysicsBody::setLinearVelocity(const FVector3f& linearVelocity)
    {
        b3Body_SetLinearVelocity(m_BodyId, convert<b3Vec3>(linearVelocity));
    }

    FVector3f FPhysicsBody::getLinearVelocity() const
    {
        return convert<FVector3f>(b3Body_GetLinearVelocity(m_BodyId));
    }

    void FPhysicsBody::setAngularVelocity(const FVector3f& angularVelocity)
    {
        b3Body_SetAngularVelocity(m_BodyId, convert<b3Vec3>(angularVelocity));
    }

    FVector3f FPhysicsBody::getAngularVelocity() const
    {
        return convert<FVector3f>(b3Body_GetAngularVelocity(m_BodyId));
    }

    void FPhysicsBody::setLinearDamping(float linearDamping)
    {
        b3Body_SetLinearDamping(m_BodyId, linearDamping);
    }

    float FPhysicsBody::getLinearDamping() const
    {
        return b3Body_GetLinearDamping(m_BodyId);
    }

    void FPhysicsBody::setAngularDamping(float angularDamping)
    {
        b3Body_SetAngularDamping(m_BodyId, angularDamping);
    }

    float FPhysicsBody::getAngularDamping() const
    {
        return b3Body_GetAngularDamping(m_BodyId);
    }

    void FPhysicsBody::setConstraints(FPhysicsConstraintsFlags constraints)
    {
        b3Body_SetMotionLocks(m_BodyId, convert<b3MotionLocks>(constraints));
    }

    FPhysicsConstraintsFlags FPhysicsBody::getConstraints() const
    {
        return convert<FPhysicsConstraintsFlags>(b3Body_GetMotionLocks(m_BodyId));
    }
}
