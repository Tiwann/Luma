#pragma once
#include "Luma/Runtime/Component.h"

namespace Luma
{
    FEntity* IComponent::getOwner() const
    {
        return m_Owner;
    }

    void IComponent::setActive(const bool active)
    {
        m_Active = active;
    }

    bool IComponent::isActive() const
    {
        return m_Active;
    }
}