#pragma once
#include "Luma/Memory/Ref.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Rendering/Texture.h"

namespace Luma
{
    class FPhysicsWorld;
    struct IRenderDevice;

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
        explicit FPhysicsWorldDebugRenderer(const FPhysicsWorld* world, IRenderDevice* renderDevice,
                                            const FPhysicsWorldDebugRendererDesc& desc =
                                                FPhysicsWorldDebugRendererDesc());

        ~FPhysicsWorldDebugRenderer() override = default;

        void destroy();

        void setPhysicsWorld(const FPhysicsWorld* world);
        IRenderDevice* getRenderDevice() const;
        void drawWorld();

    private:
        struct Impl;
        Impl* m_Pimpl = nullptr;

        const FPhysicsWorld* m_World = nullptr;
        IRenderDevice* m_RenderDevice = nullptr;
        Ref<ITexture> m_RenderTexture = nullptr;
    };
}
