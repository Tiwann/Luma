#include "Luma/Physics/PhysicsBody.h"
#include "Luma/Physics/PhysicsWorld.h"
#include "PhysicsWorldInternal.h"
#include "Conversions.h"
#include <box3d/box3d.h>

namespace Luma
{
    struct FPhysicsBody::Impl
    {
        const FPhysicsWorld* world = nullptr;
        b3BodyId bodyId = b3_nullBodyId;
        EPhysicsBodyType bodyType = EPhysicsBodyType::Static;
    };

    FPhysicsBody::FPhysicsBody(const FPhysicsWorld* world, const FPhysicsBodyDesc& desc)
    {
        LUMA_ASSERT(world, "World should be a valid pointer to a FPhysicsWorld!");
        
        m_Pimpl = new Impl();

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
        def.isAwake = desc.startsAwake;


        m_Pimpl->bodyId = b3CreateBody(world->m_Pimpl->worldId, &def);
        LUMA_ASSERT(b3Body_IsValid(m_Pimpl->bodyId), "Failed to create physics body!");

        m_Pimpl->bodyType = desc.bodyType;
        m_Pimpl->world = world;
    }

    
    void FPhysicsBody::addForce(const FVector3f& force)
    {
        b3Body_ApplyForceToCenter(m_Pimpl->bodyId, convert<b3Vec3>(force), true);
    }

    void FPhysicsBody::addForce(const FVector3f& force, const FVector3f& point)
    {
        b3Body_ApplyForce(m_Pimpl->bodyId, convert<b3Vec3>(force), convert<b3Vec3>(point), true);
    }

    void FPhysicsBody::addImpulse(const FVector3f& impulse)
    {
        b3Body_ApplyLinearImpulseToCenter(m_Pimpl->bodyId, convert<b3Vec3>(impulse), true);
    }

    void FPhysicsBody::addImpulse(const FVector3f& impulse, const FVector3f& point)
    {
        b3Body_ApplyLinearImpulse(m_Pimpl->bodyId, convert<b3Vec3>(impulse), convert<b3Vec3>(point), true);
    }

    void FPhysicsBody::addTorque(const FVector3f& torque)
    {
        b3Body_ApplyTorque(m_Pimpl->bodyId, convert<b3Vec3>(torque), true);
    }

    void FPhysicsBody::addAngularImpulse(const FVector3f& impulse)
    {
        b3Body_ApplyAngularImpulse(m_Pimpl->bodyId, convert<b3Vec3>(impulse), true);
    }



    void FPhysicsBody::destroy()
    {
        b3DestroyBody(m_Pimpl->bodyId);
        m_Pimpl->bodyId = b3_nullBodyId;
        delete m_Pimpl;
    }

    void FPhysicsBody::setBodyType(EPhysicsBodyType bodyType)
    {
        b3Body_SetType(m_Pimpl->bodyId, convert<b3BodyType>(bodyType));
    }

    EPhysicsBodyType FPhysicsBody::getBodyType() const
    {
        return convert<EPhysicsBodyType>(b3Body_GetType(m_Pimpl->bodyId));
    }

    void FPhysicsBody::setPosition(const FVector3f& position)
    {
        b3Body_SetTransform(m_Pimpl->bodyId, convert<b3Vec3>(position), b3Body_GetRotation(m_Pimpl->bodyId));
    }

    FVector3f FPhysicsBody::getPosition() const
    {
        return convert<FVector3f>(b3Body_GetPosition(m_Pimpl->bodyId));
    }

    void FPhysicsBody::setRotation(const FQuatf& rotation)
    {
        b3Body_SetTransform(m_Pimpl->bodyId, b3Body_GetPosition(m_Pimpl->bodyId), convert<b3Quat>(rotation));
    }

    FQuatf FPhysicsBody::getRotation()
    {
        return convert<FQuatf>(b3Body_GetRotation(m_Pimpl->bodyId));
    }

    void FPhysicsBody::setLinearVelocity(const FVector3f& linearVelocity)
    {
        b3Body_SetLinearVelocity(m_Pimpl->bodyId, convert<b3Vec3>(linearVelocity));
    }

    FVector3f FPhysicsBody::getLinearVelocity() const
    {
        return convert<FVector3f>(b3Body_GetLinearVelocity(m_Pimpl->bodyId));
    }

    void FPhysicsBody::setAngularVelocity(const FVector3f& angularVelocity)
    {
        b3Body_SetAngularVelocity(m_Pimpl->bodyId, convert<b3Vec3>(angularVelocity));
    }

    FVector3f FPhysicsBody::getAngularVelocity() const
    {
        return convert<FVector3f>(b3Body_GetAngularVelocity(m_Pimpl->bodyId));
    }

    void FPhysicsBody::setLinearDamping(float linearDamping)
    {
        b3Body_SetLinearDamping(m_Pimpl->bodyId, linearDamping);
    }

    float FPhysicsBody::getLinearDamping() const
    {
        return b3Body_GetLinearDamping(m_Pimpl->bodyId);
    }

    void FPhysicsBody::setAngularDamping(float angularDamping)
    {
        b3Body_SetAngularDamping(m_Pimpl->bodyId, angularDamping);
    }

    float FPhysicsBody::getAngularDamping() const
    {
        return b3Body_GetAngularDamping(m_Pimpl->bodyId);
    }

    void FPhysicsBody::setConstraints(FPhysicsConstraintsFlags constraints)
    {
        b3Body_SetMotionLocks(m_Pimpl->bodyId, convert<b3MotionLocks>(constraints));
    }

    FPhysicsConstraintsFlags FPhysicsBody::getConstraints() const
    {
        return convert<FPhysicsConstraintsFlags>(b3Body_GetMotionLocks(m_Pimpl->bodyId));
    }

    float FPhysicsBody::getMass() const
    {
        return b3Body_GetMass(m_Pimpl->bodyId);
    }

    FVector3f FPhysicsBody::getCenterOfMass() const
    {
        return convert<FVector3f>(b3Body_GetWorldCenter(m_Pimpl->bodyId));
    }

    void FPhysicsBody::setAwake(bool awake)
    {
        b3Body_SetAwake(m_Pimpl->bodyId, awake);
    }

    bool FPhysicsBody::isAwake()
    {
        return b3Body_IsAwake(m_Pimpl->bodyId);
    }
}
