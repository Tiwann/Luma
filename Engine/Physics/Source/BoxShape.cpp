#include "Luma/Physics/BoxShape.h"
#include "PhysicsShapeInternal.h"

namespace Luma
{
    BoxShape::BoxShape(const FVector3f& halfExtents)
        : m_HalfExtents(halfExtents)
    {
    }

    void BoxShape::setSize(const FVector3f& halfExtents)
    {
        m_HalfExtents = halfExtents;
        refreshNativeShape();
    }

    FVector3f BoxShape::getSize() const
    {
        return m_HalfExtents;
    }

    FPhysicsShapeGeometry BoxShape::getGeometry() const
    {
        return FPhysicsShapeGeometry{ m_HalfExtents, 0.0f, 0.0f };
    }
}
