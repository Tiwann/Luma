#pragma once
#include "PhysicsShape.h"

namespace Luma
{
    class LUMA_PHYSICS_API FSphereShape : public IPhysicsShape
    {
    public:
        FSphereShape(float radius = 0.5f);
        ~FSphereShape() override = default;

        EType getType() const override { return EType::Sphere; }

        void setRadius(float radius);
        float getRadius() const;
    private:
        FPhysicsShapeGeometry getGeometry() const override;

        float m_Radius = 0.5f;
    };
}
