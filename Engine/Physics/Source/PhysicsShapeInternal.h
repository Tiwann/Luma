#pragma once
#include <box3d/box3d.h>
#include "Luma/Physics/PhysicsShape.h"

namespace Luma
{
    struct IPhysicsShape::Impl
    {
        b3ShapeId shapeId = b3_nullShapeId;
        FPhysicsBody* body = nullptr;
        FPhysicsMaterial material;
        FVector3f localPosition = FVector3f::Zero;
        FQuatf localRotation = FQuatf::Identity;
        bool isSensor = false;
    };
}
