#pragma once
#include "PhysicsShape.h"

namespace Luma
{
    class LUMA_PHYSICS_API FBoxShape : public IPhysicsShape
    {
    public:
        FBoxShape(const FVector3f& halfExtents = FVector3f(0.5f, 0.5f, 0.5f));
        ~FBoxShape() override = default;

        EType getType() const override { return EType::Box; }

        void setSize(const FVector3f& halfExtents);
        FVector3f getSize() const;
    private:
        FPhysicsShapeGeometry getGeometry() const override;

        FVector3f m_HalfExtents;
    };
}
