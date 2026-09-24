#pragma once
#include "PhysicsShape.h"

namespace Luma
{
    class LUMA_PHYSICS_API BoxShape : public IPhysicsShape
    {
    public:
        BoxShape(const FVector3f& halfExtents = FVector3f(0.5f, 0.5f, 0.5f));
        ~BoxShape() override = default;

        EType getType() const override { return EType::Box; }

        void setSize(const FVector3f& halfExtents);
        FVector3f getSize() const;
    private:
        FPhysicsShapeGeometry getGeometry() const override;

        FVector3f m_HalfExtents;
    };
}
