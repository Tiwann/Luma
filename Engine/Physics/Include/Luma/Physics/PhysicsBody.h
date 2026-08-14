#pragma once
#include "PhysicsConstraints.h"
#include "PhysicsBodyType.h"
#include "Luma/Math/Quaternion.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Physics/Export.h"

namespace Luma
{
    class IPhysicsShape;
    class FPhysicsWorld;


    struct FPhysicsBodyDesc
    {
        EPhysicsBodyType bodyType = EPhysicsBodyType::Dynamic;
        FVector3f initialPosition = FVector3f::Zero;
        FQuatf initialRotation = FQuatf::Identity;
        FVector3f initialLinearVelocity = FVector3f::Zero;
        FVector3f initialAngularVelocity = FVector3f::Zero;
        float linearDamping = 0.0f;
        float angularDamping = 0.0f;
        bool startsAwake = true;
        FPhysicsConstraintsFlags constraints = EPhysicsConstraintsBits::None;
    };

    class LUMA_PHYSICS_API FPhysicsBody : public IRefCounted<FPhysicsBody>
    {
    public:
        explicit FPhysicsBody(const FPhysicsWorld* world, const FPhysicsBodyDesc& desc = FPhysicsBodyDesc());
        ~FPhysicsBody() override = default;

        void destroy();

        void attachShape(Ref<IPhysicsShape> shape, bool updateMass = true);
        void detachShape(IPhysicsShape* shape, bool updateMass = true);
        void detachAllShapes();
        uint32_t getShapeCount() const;
        IPhysicsShape* getShape(uint32_t index) const;

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

        float getMass() const;
        FVector3f getCenterOfMass() const;

        void setAwake(bool awake);
        bool isAwake();

        void addForce(const FVector3f& force);
        void addForce(const FVector3f& force, const FVector3f& point);
        void addImpulse(const FVector3f& impulse);
        void addImpulse(const FVector3f& impulse, const FVector3f& point);
        void addTorque(const FVector3f& torque);
        void addAngularImpulse(const FVector3f& impulse);
    private:
        friend class IPhysicsShape;

        void createNativeShape(IPhysicsShape* shape);
        void refreshShape(IPhysicsShape* shape);

        struct Impl;
        Impl* m_Pimpl = nullptr;
    };
}
