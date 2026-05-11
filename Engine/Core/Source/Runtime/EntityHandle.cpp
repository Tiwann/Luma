#include "Luma/Runtime/EntityHandle.h"
#include "Luma/Runtime/Scene.h"

namespace Luma
{
    FEntityHandle::FEntityHandle(uint32_t handle, FScene* context): m_Handle(handle), m_Context(context)
    {}

    FEntityHandle::FEntityHandle(decltype(nullptr)): m_Handle(0), m_Context(nullptr)
    {}


    FEntity* FEntityHandle::operator->()
    {
        if (!m_Context) return nullptr;
        FEntity** entity = m_Context->m_Entities.findValue(m_Handle);
        if (!entity) return nullptr;
        return *entity;
    }
}
