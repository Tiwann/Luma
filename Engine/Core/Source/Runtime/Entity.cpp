#include "Luma/Runtime/Entity.h"

namespace Luma
{
    void Entity::onInit()
    {
        for (auto* component : m_Components)
            component->initialize();
    }

    void Entity::onDestroy()
    {
        for (auto* component : m_Components)
        {
            component->destroy();
            delete component;
        }
        m_Components.clear();
    }

    void Entity::onUpdate(double deltaTime)
    {
        for (auto* component : m_Components)
        {
            if (component->isActive())
                component->onUpdate(deltaTime);
        }
    }

    void Entity::onPhysicsUpdate(double deltaTime)
    {
        for (auto* component : m_Components)
        {
            if (component->isActive())
                component->onPhysicsUpdate(deltaTime);
        }
    }

    void Entity::onLateUpdate(double deltaTime)
    {
        for (auto* component : m_Components)
        {
            if (component->isActive())
                component->onLateUpdate(deltaTime);
        }
    }

    void Entity::onRender(CommandBuffer* cmdBuffer)
    {
        for (auto* component : m_Components)
        {
            if (component->isActive())
                component->onRender(cmdBuffer);
        }
    }

    Entity* Entity::getParent() const
    {
        return m_Parent;
    }

    void Entity::setParent(Entity* parent)
    {
        m_Parent = parent;
    }

    void Entity::addChild(Entity* child)
    {
        m_Children.addUnique(child);
    }

    void Entity::removeChild(Entity* child)
    {
        m_Children.remove(child);
    }

    FScene* Entity::getOwner() const
    {
        return m_Owner;
    }

    AssetType Entity::getAssetType() const
    {
        return AssetType::Entity;
    }

    bool Entity::isActive() const
    {
        return m_Active;
    }

    void Entity::setActive(bool active)
    {
        m_Active = active;
    }

    void Entity::initialize()
    {
        onInit();
    }

    void Entity::destroy()
    {
        onDestroy();
    }

    const Array<Entity*>& Entity::getChildren() const
    {
        return m_Children;
    }
}
