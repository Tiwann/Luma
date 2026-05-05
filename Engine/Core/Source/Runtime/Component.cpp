#pragma once
#include "Luma/Runtime/Component.h"
#include "Luma/Runtime/Entity.h"
#include "Luma/Components/TransformComponent.h"

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

    FTransformComponent* IComponent::getTransformComponent() const
    {
        const FEntity* owner = getOwner();
        if (!owner) return nullptr;
        return owner->getComponent<FTransformComponent>();
    }
}
