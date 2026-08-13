#pragma once
#include "PhysicsBody.h"
#include "Luma/Containers/Array.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Physics/Export.h"

#include <box3d/id.h>


namespace Luma
{
    struct FPhysicsWorldDesc
    {
        FVector3f gravity = FVector3f(0.0f, -9.81f, 0.0f);
    };

    class LUMA_PHYSICS_API FPhysicsWorld : public IRefCounted<FPhysicsWorld>
    {
    public:
        FPhysicsWorld() = default;
        ~FPhysicsWorld() override = default;
        static constexpr float TIMESTEP = 1.0f / 60.0f;

        bool initialize(const FPhysicsWorldDesc& desc);
        void destroy();
        void step(float timeStep = TIMESTEP, uint32_t substepCount = 4);

        FPhysicsBody* createBody(const FPhysicsBodyDesc& desc);
        void destroyBody(FPhysicsBody* body);

        b3WorldId getWorldId() const { return m_WorldId; }

        void setGravity(const FVector3f& gravity);
        FVector3f getGravity() const;
    private:
        b3WorldId m_WorldId {b3_nullWorldId};
        TArray<FPhysicsBody*> m_Bodies;
    };
}
