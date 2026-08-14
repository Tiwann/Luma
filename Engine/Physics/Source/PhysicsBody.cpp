#include "Luma/Physics/PhysicsBody.h"
#include "Luma/Physics/PhysicsWorld.h"
#include "PhysicsWorldInternal.h"
#include "PhysicsShapeInternal.h"
#include "Conversions.h"
#include <box3d/box3d.h>

#include "Luma/Physics/PhysicsShape.h"
#include "Luma/Physics/BoxShape.h"
#include "Luma/Physics/SphereShape.h"
#include "Luma/Physics/PlaneShape.h"
#include "Luma/Containers/Array.h"
#include "Luma/Memory/Ref.h"

namespace Luma
{
    namespace
    {
        b3Quat makePlaneRotation(const b3Vec3& normal)
        {
            const b3Vec3 cross = b3Cross(b3Vec3_axisY, normal);
            if (b3LengthSquared(cross) < 1.0e-6f)
            {
                if (normal.y >= 0.0f)
                    return b3Quat_identity;
                return b3MakeQuatFromAxisAngle(b3Vec3_axisX, B3_PI);
            }

            b3Quat rotation;
            rotation.v = cross;
            rotation.s = 1.0f + b3Dot(b3Vec3_axisY, normal);
            return b3NormalizeQuat(rotation);
        }
    }
    struct FPhysicsBody::Impl
    {
        const FPhysicsWorld* world = nullptr;
        b3BodyId bodyId = b3_nullBodyId;
        EPhysicsBodyType bodyType = EPhysicsBodyType::Static;
        TArray<Ref<IPhysicsShape>> shapes;
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
        if (m_Pimpl == nullptr)
            return;

        if (b3Body_IsValid(m_Pimpl->bodyId))
            b3DestroyBody(m_Pimpl->bodyId);
        m_Pimpl->bodyId = b3_nullBodyId;

        for (Ref<IPhysicsShape> shape : m_Pimpl->shapes)
        {
            shape->m_Pimpl->shapeId = b3_nullShapeId;
            shape->m_Pimpl->body = nullptr;
        }
        m_Pimpl->shapes.free();

        delete m_Pimpl;
        m_Pimpl = nullptr;
    }

    void FPhysicsBody::createNativeShape(IPhysicsShape* shape)
    {
        IPhysicsShape::Impl& shapeImpl = *shape->m_Pimpl;

        b3ShapeDef def = b3DefaultShapeDef();
        def.userData = this;
        def.density = shapeImpl.material.density;
        def.baseMaterial = b3DefaultSurfaceMaterial();
        def.baseMaterial.friction = shapeImpl.material.friction;
        def.baseMaterial.restitution = shapeImpl.material.restitution;
        def.isSensor = shapeImpl.isSensor;
        def.updateBodyMass = false;

        b3ShapeId shapeId = b3_nullShapeId;

        switch (shape->getType())
        {
        case IPhysicsShape::EType::Box:
        {
            const FPhysicsShapeGeometry geometry = shape->getGeometry();

            b3Transform transform;
            transform.p = convert<b3Vec3>(shapeImpl.localPosition);
            transform.q = convert<b3Quat>(shapeImpl.localRotation);

            b3BoxHull hull = b3MakeTransformedBoxHull(geometry.halfExtents.x, geometry.halfExtents.y, geometry.halfExtents.z, transform);
            shapeId = b3CreateHullShape(m_Pimpl->bodyId, &def, &hull.base);
            break;
        }
        case IPhysicsShape::EType::Sphere:
        {
            const FPhysicsShapeGeometry geometry = shape->getGeometry();

            b3Sphere sphere;
            sphere.center = convert<b3Vec3>(shapeImpl.localPosition);
            sphere.radius = geometry.radius;
            shapeId = b3CreateSphereShape(m_Pimpl->bodyId, &def, &sphere);
            break;
        }
        case IPhysicsShape::EType::Plane:
        {
            const FPhysicsShapeGeometry geometry = shape->getGeometry();

            b3Transform transform;
            transform.p = b3MulSV(geometry.offset, convert<b3Vec3>(geometry.normal));
            transform.q = makePlaneRotation(convert<b3Vec3>(geometry.normal));

            b3BoxHull hull = b3MakeTransformedBoxHull(geometry.halfExtents.x, geometry.halfExtents.y, geometry.halfExtents.z, transform);
            shapeId = b3CreateHullShape(m_Pimpl->bodyId, &def, &hull.base);
            break;
        }
        }

        shapeImpl.shapeId = shapeId;
    }

    void FPhysicsBody::attachShape(Ref<IPhysicsShape> shape, bool updateMass)
    {
        if (!shape) return;

        IPhysicsShape::Impl& shapeImpl = *shape->m_Pimpl;
        if (shapeImpl.body)
        {
            LUMA_ASSERT(false, "Shape is already attached to a body!");
            return;
        }

        shapeImpl.body = this;
        createNativeShape(shape);
        LUMA_ASSERT(b3Shape_IsValid(shapeImpl.shapeId), "Failed to create physics shape!");

        if (updateMass)
            b3Body_ApplyMassFromShapes(m_Pimpl->bodyId);

        m_Pimpl->shapes.add(shape);
    }

    void FPhysicsBody::detachShape(IPhysicsShape* shape, bool updateMass)
    {
        if (!shape || shape->m_Pimpl->body != this) return;

        if (b3Shape_IsValid(shape->m_Pimpl->shapeId))
            b3DestroyShape(shape->m_Pimpl->shapeId, updateMass);
        shape->m_Pimpl->shapeId = b3_nullShapeId;
        shape->m_Pimpl->body = nullptr;

        for (uint32_t i = 0; i < m_Pimpl->shapes.count(); ++i)
        {
            if (m_Pimpl->shapes[i] == shape)
            {
                m_Pimpl->shapes.removeAt(i);
                break;
            }
        }
    }

    void FPhysicsBody::detachAllShapes()
    {
        for (Ref<IPhysicsShape> shape : m_Pimpl->shapes)
        {
            if (b3Shape_IsValid(shape->m_Pimpl->shapeId))
                b3DestroyShape(shape->m_Pimpl->shapeId, true);
            shape->m_Pimpl->shapeId = b3_nullShapeId;
            shape->m_Pimpl->body = nullptr;
        }
        m_Pimpl->shapes.clear();
    }

    uint32_t FPhysicsBody::getShapeCount() const
    {
        return m_Pimpl->shapes.count();
    }

    IPhysicsShape* FPhysicsBody::getShape(uint32_t index) const
    {
        return m_Pimpl->shapes[index];
    }

    void FPhysicsBody::refreshShape(IPhysicsShape* shape)
    {
        if (!shape || shape->m_Pimpl->body != this) return;

        if (b3Shape_IsValid(shape->m_Pimpl->shapeId))
            b3DestroyShape(shape->m_Pimpl->shapeId, false);
        shape->m_Pimpl->shapeId = b3_nullShapeId;

        createNativeShape(shape);
        LUMA_ASSERT(b3Shape_IsValid(shape->m_Pimpl->shapeId), "Failed to refresh physics shape!");

        b3Body_ApplyMassFromShapes(m_Pimpl->bodyId);
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
