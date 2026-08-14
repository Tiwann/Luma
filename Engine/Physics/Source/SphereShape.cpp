#include "Luma/Physics/SphereShape.h"
#include "PhysicsShapeInternal.h"

namespace Luma
{
    FSphereShape::FSphereShape(float radius)
        : m_Radius(radius)
    {
    }

    void FSphereShape::setRadius(float radius)
    {
        m_Radius = radius;
        refreshNativeShape();
    }

    float FSphereShape::getRadius() const
    {
        return m_Radius;
    }

    FPhysicsShapeGeometry FSphereShape::getGeometry() const
    {
        return FPhysicsShapeGeometry{ FVector3f::Zero, m_Radius, 0.0f };
    }
}
