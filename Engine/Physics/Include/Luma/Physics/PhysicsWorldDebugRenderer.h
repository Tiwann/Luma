#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Rendering/Texture.h"

namespace Luma
{
    class FPhysicsWorld;
    struct IGpuDevice;

    struct FPhysicsWorldDebugRendererDesc
    {
        float forceScale = 1.0f;
        float jointScale = 1.0f;
        bool drawShapes = true;
        bool drawJoints = false;
        bool drawJointExtras = false;
        bool drawBounds = false;
        bool drawMass = false;
    };

    class FPhysicsWorldDebugRenderer : public IRefCounted<FPhysicsWorldDebugRenderer>
    {
    public:
        explicit FPhysicsWorldDebugRenderer(const FPhysicsWorld* world, IGpuDevice* gpuDevice,
                                            const FPhysicsWorldDebugRendererDesc& desc =
                                                FPhysicsWorldDebugRendererDesc());

        ~FPhysicsWorldDebugRenderer() override = default;

        void destroy();

        void setPhysicsWorld(const FPhysicsWorld* world);
        IGpuDevice* getGpuDevice() const;
        void drawWorld();

    private:
        struct Impl;
        Impl* m_Pimpl = nullptr;

        const FPhysicsWorld* m_World = nullptr;
        IGpuDevice* m_GpuDevice = nullptr;
        Ref<ITexture> m_RenderTexture = nullptr;
    };
}
