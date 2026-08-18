#pragma once
#include "PhysicsMaterial.h"
#include "Luma/Math/Quaternion.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Physics/Export.h"

namespace Luma
{
    class FPhysicsBody;

    struct FPhysicsShapeGeometry
    {
        FVector3f halfExtents = FVector3f::Zero;
        float radius = 0.0f;
        float height = 0.0f;
        FVector3f normal = FVector3f::Zero;
        float offset = 0.0f;
    };

    class LUMA_PHYSICS_API IPhysicsShape : public IRefCounted<IPhysicsShape>
    {
    public:
        enum class EType
        {
            Box,
            Sphere,
            Plane
        };

        ~IPhysicsShape() override = default;

        void destroy();

        virtual EType getType() const = 0;

        void setLocalPosition(const FVector3f& position);
        FVector3f getLocalPosition() const;

        void setLocalRotation(const FQuatf& rotation);
        FQuatf getLocalRotation() const;

        void setPhysicsMaterial(const FPhysicsMaterial& material);
        const FPhysicsMaterial& getPhysicsMaterial() const;

        void setSensor(bool sensor);
        bool isSensor() const;
    protected:
        IPhysicsShape();

        void refreshNativeShape();
    private:
        friend class FPhysicsBody;

        virtual FPhysicsShapeGeometry getGeometry() const = 0;

        struct Impl;
        Impl* m_Pimpl = nullptr;
    };
}
