#pragma once
#include "PhysicsBody.h"
#include "Luma/Containers/Array.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Physics/Export.h"

namespace Luma
{
    struct PhysicsWorldDesc
    {
        FVector3f gravity = FVector3f(0.0f, -9.81f, 0.0f);
        float timestep = 1.0f / 60.0f;
        uint32_t substepCount = 4;
        uint32_t maxBodies = 1024;
    };

    class LUMA_PHYSICS_API PhysicsWorld : public RefCounted<PhysicsWorld>
    {
    public:
        explicit PhysicsWorld(const PhysicsWorldDesc& desc = PhysicsWorldDesc());
        ~PhysicsWorld() override = default;

        void destroy();
        void step();

        Ref<PhysicsBody> createBody(const FPhysicsBodyDesc& desc = FPhysicsBodyDesc());
        void destroyBody(Ref<PhysicsBody> body);

        void setGravity(const FVector3f& gravity);
        FVector3f getGravity() const;

        void setTimestep(float timestep);
        float getTimestep();
    private:
        struct Impl;
        friend class PhysicsBody;
        friend class FPhysicsWorldDebugRenderer;

        Impl* m_Pimpl = nullptr;
    };

    LUMA_PHYSICS_API PhysicsWorld* createPhysicsWorld(const PhysicsWorldDesc& desc = PhysicsWorldDesc());
}
