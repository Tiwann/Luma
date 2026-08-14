#include "Luma/Physics/BoxShape.h"
#include "PhysicsShapeInternal.h"

namespace Luma
{
    FBoxShape::FBoxShape(const FVector3f& halfExtents)
        : m_HalfExtents(halfExtents)
    {
    }

    void FBoxShape::setSize(const FVector3f& halfExtents)
    {
        m_HalfExtents = halfExtents;
        refreshNativeShape();
    }

    FVector3f FBoxShape::getSize() const
    {
        return m_HalfExtents;
    }

    FPhysicsShapeGeometry FBoxShape::getGeometry() const
    {
        return FPhysicsShapeGeometry{ m_HalfExtents, 0.0f, 0.0f };
    }
}
