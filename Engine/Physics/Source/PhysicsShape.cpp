#include "Luma/Physics/PhysicsShape.h"
#include "PhysicsShapeInternal.h"
#include "Luma/Physics/PhysicsBody.h"

namespace Luma
{
    IPhysicsShape::IPhysicsShape()
    {
        m_Pimpl = new Impl();
    }

    void IPhysicsShape::destroy()
    {
        if (m_Pimpl == nullptr)
            return;

        if (m_Pimpl->body && b3Shape_IsValid(m_Pimpl->shapeId))
            b3DestroyShape(m_Pimpl->shapeId, true);

        m_Pimpl->body = nullptr;
        m_Pimpl->shapeId = b3_nullShapeId;
        delete m_Pimpl;
        m_Pimpl = nullptr;
    }

    void IPhysicsShape::refreshNativeShape()
    {
        if (m_Pimpl && m_Pimpl->body)
            m_Pimpl->body->refreshShape(this);
    }

    void IPhysicsShape::setLocalPosition(const FVector3f& position)
    {
        m_Pimpl->localPosition = position;
        refreshNativeShape();
    }

    FVector3f IPhysicsShape::getLocalPosition() const
    {
        return m_Pimpl->localPosition;
    }

    void IPhysicsShape::setLocalRotation(const FQuatf& rotation)
    {
        m_Pimpl->localRotation = rotation;
        refreshNativeShape();
    }

    FQuatf IPhysicsShape::getLocalRotation() const
    {
        return m_Pimpl->localRotation;
    }

    void IPhysicsShape::setPhysicsMaterial(const FPhysicsMaterial& material)
    {
        m_Pimpl->material = material;
        refreshNativeShape();
    }

    const FPhysicsMaterial& IPhysicsShape::getPhysicsMaterial() const
    {
        return m_Pimpl->material;
    }

    void IPhysicsShape::setSensor(bool sensor)
    {
        m_Pimpl->isSensor = sensor;
        refreshNativeShape();
    }

    bool IPhysicsShape::isSensor() const
    {
        return m_Pimpl->isSensor;
    }
}
