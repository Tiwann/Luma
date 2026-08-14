#include "Luma/Physics/PlaneShape.h"
#include "PhysicsShapeInternal.h"

namespace Luma
{
    FPlaneShape::FPlaneShape(const FVector3f& normal, float offset, const FVector3f& halfExtents)
        : m_Normal(normal), m_Offset(offset), m_HalfExtents(halfExtents)
    {
    }

    void FPlaneShape::setNormal(const FVector3f& normal)
    {
        m_Normal = normal;
        refreshNativeShape();
    }

    FVector3f FPlaneShape::getNormal() const
    {
        return m_Normal;
    }

    void FPlaneShape::setOffset(float offset)
    {
        m_Offset = offset;
        refreshNativeShape();
    }

    float FPlaneShape::getOffset() const
    {
        return m_Offset;
    }

    void FPlaneShape::setHalfExtents(const FVector3f& halfExtents)
    {
        m_HalfExtents = halfExtents;
        refreshNativeShape();
    }

    FVector3f FPlaneShape::getHalfExtents() const
    {
        return m_HalfExtents;
    }

    FPhysicsShapeGeometry FPlaneShape::getGeometry() const
    {
        return FPhysicsShapeGeometry{ m_HalfExtents, 0.0f, 0.0f, m_Normal, m_Offset };
    }
}
