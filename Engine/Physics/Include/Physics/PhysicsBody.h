#pragma once
#include "PhysicsConstraints.h"
#include "PhysicsBodyType.h"
#include "Luma/Math/Quaternion.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Physics/Export.h"
#include <box3d/id.h>


namespace Luma
{
    class FPhysicsWorld;


    struct FPhysicsBodyDesc
    {
        FPhysicsWorld* world = nullptr;
        EPhysicsBodyType bodyType = EPhysicsBodyType::Static;
        FVector3f initialPosition = FVector3f::Zero;
        FQuatf initialRotation = FQuatf::Identity;
        FVector3f initialLinearVelocity = FVector3f::Zero;
        FVector3f initialAngularVelocity = FVector3f::Zero;
        float linearDamping = 0.0f;
        float angularDamping = 0.0f;
        FPhysicsConstraintsFlags constraints = EPhysicsConstraintsBits::None;
    };

    class LUMA_PHYSICS_API FPhysicsBody : public IRefCounted<FPhysicsBody>
    {
    public:
        bool initialize(const FPhysicsBodyDesc& desc);
        void destroy();

        void setBodyType(EPhysicsBodyType bodyType);
        EPhysicsBodyType getBodyType() const;

        void setPosition(const FVector3f& position);
        FVector3f getPosition() const;
        void setRotation(const FQuatf& rotation);
        FQuatf getRotation();
        void setLinearVelocity(const FVector3f& linearVelocity);
        FVector3f getLinearVelocity() const;
        void setAngularVelocity(const FVector3f& angularVelocity);
        FVector3f getAngularVelocity() const;
        void setLinearDamping(float linearDamping);
        float getLinearDamping() const;
        void setAngularDamping(float angularDamping);
        float getAngularDamping() const;

        void setConstraints(FPhysicsConstraintsFlags constraints);
        FPhysicsConstraintsFlags getConstraints() const;
    private:
        FPhysicsWorld* m_World = nullptr;
        b3BodyId m_BodyId{b3_nullBodyId};
        EPhysicsBodyType m_BodyType = EPhysicsBodyType::Static;
    };
}
