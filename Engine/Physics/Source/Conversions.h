#pragma once
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Quaternion.h"
#include <box3d/math_functions.h>
#include <box3d/types.h>


namespace Luma
{
    template<typename OutType, typename InType>
    OutType convert(const InType& value);

    template<>
    inline b3Vec3 convert(const FVector3f& value)
    {
        return b3Vec3(value.x, value.y, value.z);
    }

    template<>
    inline FVector3f convert(const b3Vec3& value)
    {
        return FVector3f(value.x, value.y, value.z);
    }

    template<>
    inline b3Quat convert(const FQuatf& value)
    {
        return b3Quat({value.x, value.y, value.z}, value.w);
    }

    template<>
    inline FQuatf convert(const b3Quat& value)
    {
        return FQuatf(value.v.x, value.v.y, value.v.z, value.s);
    }

    template<>
    inline b3MotionLocks convert(const FPhysicsConstraintsFlags& value)
    {
        b3MotionLocks locks;
        locks.linearX = value & EPhysicsConstraintsBits::PositionX;
        locks.linearY = value & EPhysicsConstraintsBits::PositionY;
        locks.linearZ = value & EPhysicsConstraintsBits::PositionZ;

        locks.angularX = value & EPhysicsConstraintsBits::RotationX;
        locks.angularY = value & EPhysicsConstraintsBits::RotationY;
        locks.angularZ = value & EPhysicsConstraintsBits::RotationZ;
        return locks;
    }

    template<>
    inline FPhysicsConstraintsFlags convert(const b3MotionLocks& value)
    {
        FPhysicsConstraintsFlags constraints;
        if (value.linearX) constraints |= EPhysicsConstraintsBits::PositionX;
        if (value.linearY) constraints |= EPhysicsConstraintsBits::PositionY;
        if (value.linearZ) constraints |= EPhysicsConstraintsBits::PositionZ;

        if (value.angularX) constraints |= EPhysicsConstraintsBits::RotationX;
        if (value.angularY) constraints |= EPhysicsConstraintsBits::RotationY;
        if (value.angularZ) constraints |= EPhysicsConstraintsBits::RotationZ;
        return constraints;
    }

    template<>
    inline b3BodyType convert(const EPhysicsBodyType& value)
    {
        switch (value)
        {
        case EPhysicsBodyType::Static: return b3_staticBody;
        case EPhysicsBodyType::Kinematic: return b3_kinematicBody;
        case EPhysicsBodyType::Dynamic: return b3_dynamicBody;
        default: throw;
        }
    }

    template<>
    inline EPhysicsBodyType convert(const b3BodyType& value)
    {
        switch (value)
        {
        case b3_staticBody : return EPhysicsBodyType::Static;
        case b3_kinematicBody : return EPhysicsBodyType::Kinematic;
        case b3_dynamicBody : return EPhysicsBodyType::Dynamic;
        default: throw;
        }
    }
}
