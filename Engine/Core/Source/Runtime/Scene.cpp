#include "Luma/Runtime/Scene.h"
#include "Luma/Runtime/Entity.h"
#include "Luma/Runtime/EntityHandle.h"
#include "Luma/Components/TransformComponent.h"

LUMA_DEFINE_CLASS(Luma::Scene)

namespace Luma
{
    FEntityHandle Scene::createEntity()
    {
        Entity* entity = new Entity(this);
        const uint32_t id = s_EntityId;
        m_Entities[s_EntityId++] = entity;
        entity->addComponent<TransformComponent>();
        entity->initialize();
        return FEntityHandle(id, this);
    }

    void Scene::destroyEntity(FEntityHandle entity)
    {
        LUMA_ASSERT(entity.getContext() == this, "Entity context don't match!");
        Entity** pEntity = m_Entities.findValue(entity.getHandle());
        if (!pEntity) return;
        (*pEntity)->destroy();
        delete *pEntity;
        m_Entities.remove(entity.getHandle());
    }

    Array<Entity*> Scene::getEntities()
    {
        Array<Entity*> entities;
        for (auto& [_, entity] : m_Entities)
            entities.addUnique(entity);
        return entities;
    }

    void Scene::initialize()
    {
        for (auto& [_, entity] : m_Entities)
        {
            if (entity->isActive())
                entity->initialize();
        }
    }

    void Scene::destroy()
    {
        for (auto& [_, entity] : m_Entities)
        {
            entity->destroy();
            delete entity;
        }
        m_Entities.clear();
    }

    void Scene::onUpdate(double deltaTime)
    {
        for (auto& [_, entity] : m_Entities)
        {
            if (entity->isActive())
                entity->onUpdate(deltaTime);
        }
    }

    void Scene::onPhysicsUpdate(double deltaTime)
    {
        for (auto& [_, entity] : m_Entities)
        {
            if (entity->isActive())
                entity->onPhysicsUpdate(deltaTime);
        }
    }

    void Scene::onLateUpdate(double deltaTime)
    {
        for (auto& [_, entity] : m_Entities)
        {
            if (entity->isActive())
                entity->onLateUpdate(deltaTime);
        }
    }

    void Scene::onRender(RHI::CommandBuffer* cmdBuffer)
    {
        for (auto& [_, entity] : m_Entities)
        {
            if (entity->isActive())
                entity->onRender(cmdBuffer);
        }
    }
}
