#pragma once
#include "PhysicsShape.h"

namespace Luma
{
    class LUMA_PHYSICS_API FPlaneShape : public IPhysicsShape
    {
    public:
        FPlaneShape(const FVector3f& normal = FVector3f(0.0f, 1.0f, 0.0f), float offset = 0.0f, const FVector3f& halfExtents = FVector3f(50.0f, 0.1f, 50.0f));
        ~FPlaneShape() override = default;

        EType getType() const override { return EType::Plane; }

        void setNormal(const FVector3f& normal);
        FVector3f getNormal() const;

        void setOffset(float offset);
        float getOffset() const;

        void setHalfExtents(const FVector3f& halfExtents);
        FVector3f getHalfExtents() const;
    private:
        FPhysicsShapeGeometry getGeometry() const override;

        FVector3f m_Normal;
        float m_Offset;
        FVector3f m_HalfExtents;
    };
}
