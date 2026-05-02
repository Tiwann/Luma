#include "Luma/Runtime/Entity.h"

namespace Luma
{
    FEntity::FEntity(const uint32_t handle, FScene* context) : m_Context(context), m_Handle(handle)
    {
    }

    FEntity FEntity::getParent() const
    {
        return m_ParentHandle != 0 ? FEntity(m_ParentHandle, m_Context) : nullptr;
    }

    void FEntity::setParent(FEntity parent)
    {
        m_ParentHandle = parent.m_Handle;
    }

    TArray<FEntity> FEntity::getChildren() const
    {
        return m_ChildrenHandles.transform<FEntity>([this](const uint32_t handle){ return FEntity(handle, m_Context); });
    }

    void FEntity::addChild(FEntity child)
    {
        m_ChildrenHandles.add(child.m_Handle);
    }

    void FEntity::removeChild(FEntity child)
    {
        m_ChildrenHandles.remove(child.m_Handle);
    }

    bool FEntity::operator==(const FEntity& other) const
    {
        return m_Context == other.m_Context && m_Handle == other.m_Handle;
    }
}
