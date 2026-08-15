#include "Luma/Physics/PhysicsWorldDebugRenderer.h"
#include "Luma/Physics/PhysicsWorld.h"
#include "PhysicsWorldInternal.h"
#include <box3d/box3d.h>


namespace Luma
{
    struct FPhysicsWorldDebugRenderer::Impl
    {
        b3DebugDraw debugDraw = b3DefaultDebugDraw();
    };

    static void drawShape(void* userShape, b3WorldTransform transform, b3HexColor color, void* context)
    {
        FPhysicsWorldDebugRenderer* debugRenderer = static_cast<FPhysicsWorldDebugRenderer*>(context);
        IRenderDevice* renderDevice = debugRenderer->getRenderDevice();

    }

    FPhysicsWorldDebugRenderer::FPhysicsWorldDebugRenderer(const FPhysicsWorld* world, IRenderDevice* renderDevice,
        const FPhysicsWorldDebugRendererDesc& desc)
            : m_World(world), m_RenderDevice(renderDevice)
    {
        m_Pimpl = new Impl;
        m_Pimpl->debugDraw.context = this;
        m_Pimpl->debugDraw.drawShapes = desc.drawShapes;
        m_Pimpl->debugDraw.DrawShapeFcn = drawShape;
    }

    void FPhysicsWorldDebugRenderer::destroy()
    {
        delete m_Pimpl;
    }

    void FPhysicsWorldDebugRenderer::setPhysicsWorld(const FPhysicsWorld* world)
    {
        m_World = world;
    }

    IRenderDevice* FPhysicsWorldDebugRenderer::getRenderDevice() const
    {
        return m_RenderDevice;
    }

    void FPhysicsWorldDebugRenderer::drawWorld()
    {
        if (!m_World) return;

        b3WorldId worldId = m_World->m_Pimpl->worldId;
        b3World_Draw(worldId, &m_Pimpl->debugDraw, UINT64_MAX);
    }
}
