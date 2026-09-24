#include "Luma/Runtime/Component.h"
#include "Luma/Runtime/Entity.h"
#include "Luma/Components/TransformComponent.h"

namespace Luma
{
    Entity* Component::getOwner() const
    {
        return m_Owner;
    }

    void Component::setActive(const bool active)
    {
        m_Active = active;
    }

    bool Component::isActive() const
    {
        return m_Active;
    }

    TransformComponent* Component::getTransformComponent() const
    {
        const Entity* owner = getOwner();
        if (!owner) return nullptr;
        return owner->getComponent<TransformComponent>();
    }
}
