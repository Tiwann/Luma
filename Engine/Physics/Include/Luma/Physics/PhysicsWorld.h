#pragma once
#include "PhysicsBody.h"
#include "Luma/Containers/Array.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Physics/Export.h"

namespace Luma
{
    struct FPhysicsWorldDesc
    {
        FVector3f gravity = FVector3f(0.0f, -9.81f, 0.0f);
        float timestep = 1.0f / 60.0f;
        uint32_t substepCount = 4;
        uint32_t maxBodies = 1024;
    };

    class LUMA_PHYSICS_API FPhysicsWorld : public IRefCounted<FPhysicsWorld>
    {
    public:
        explicit FPhysicsWorld(const FPhysicsWorldDesc& desc = FPhysicsWorldDesc());
        ~FPhysicsWorld() override = default;

        void destroy();
        void step();

        Ref<FPhysicsBody> createBody(const FPhysicsBodyDesc& desc = FPhysicsBodyDesc());
        void destroyBody(Ref<FPhysicsBody> body);

        void setGravity(const FVector3f& gravity);
        FVector3f getGravity() const;

        void setTimestep(float timestep);
        float getTimestep();
    private:
        struct Impl;
        friend class FPhysicsBody;
        friend class FPhysicsWorldDebugRenderer;

        Impl* m_Pimpl = nullptr;
    };
}
