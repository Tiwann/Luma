#include "Luma/Runtime/Entity.h"

namespace Luma
{
    void FEntity::onInit()
    {
        for (auto* component : m_Components)
            component->initialize();
    }

    void FEntity::onDestroy()
    {
        for (auto* component : m_Components)
            component->destroy();
        m_Components.clear();
    }

    void FEntity::onUpdate(double deltaTime)
    {
        for (auto* component : m_Components)
        {
            if (component->isActive())
                component->onUpdate(deltaTime);
        }
    }

    void FEntity::onPhysicsUpdate(double deltaTime)
    {
        for (auto* component : m_Components)
        {
            if (component->isActive())
                component->onPhysicsUpdate(deltaTime);
        }
    }

    void FEntity::onLateUpdate(double deltaTime)
    {
        for (auto* component : m_Components)
        {
            if (component->isActive())
                component->onLateUpdate(deltaTime);
        }
    }

    void FEntity::onRender(ICommandBuffer* cmdBuffer)
    {
        for (auto* component : m_Components)
        {
            if (component->isActive())
                component->onRender(cmdBuffer);
        }
    }

    FEntity* FEntity::getParent() const
    {
        return m_Parent;
    }

    void FEntity::setParent(FEntity* parent)
    {
        m_Parent = parent;
    }

    void FEntity::addChild(FEntity* child)
    {
        m_Children.addUnique(child);
    }

    void FEntity::removeChild(FEntity* child)
    {
        m_Children.remove(child);
    }

    FScene* FEntity::getOwner() const
    {
        return m_Owner;
    }

    EAssetType FEntity::getAssetType() const
    {
        return EAssetType::Entity;
    }

    bool FEntity::isActive() const
    {
        return m_Active;
    }

    void FEntity::setActive(bool active)
    {
        m_Active = active;
    }

    void FEntity::initialize()
    {
        onInit();
    }

    void FEntity::destroy()
    {
        onDestroy();
    }

    const TArray<FEntity*>& FEntity::getChildren() const
    {
        return m_Children;
    }
}
