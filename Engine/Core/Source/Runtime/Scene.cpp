#include "Luma/Runtime/Scene.h"
#include "Luma/Runtime/Entity.h"
#include "Luma/Runtime/EntityHandle.h"
#include "Luma/Components/TransformComponent.h"

namespace Luma
{
    FEntityHandle FScene::createEntity()
    {
        FEntity* entity = new FEntity(this);
        const uint32_t id = s_EntityId;
        m_Entities[s_EntityId++] = entity;
        entity->addComponent<FTransformComponent>();
        entity->initialize();
        return FEntityHandle(id, this);
    }

    void FScene::destroyEntity(FEntityHandle entity)
    {
        LUMA_ASSERT(entity.getContext() == this, "Entity context don't match!");
        FEntity** pEntity = m_Entities.findValue(entity.getHandle());
        if (!pEntity) return;
        (*pEntity)->destroy();
        delete *pEntity;
        m_Entities.remove(entity.getHandle());
    }

    void FScene::initialize()
    {
        for (auto& [_, entity] : m_Entities)
        {
            if (entity->isActive())
                entity->initialize();
        }
    }

    void FScene::destroy()
    {
        for (auto& [_, entity] : m_Entities)
        {
            entity->destroy();
            delete entity;
        }
        m_Entities.clear();
    }

    void FScene::onUpdate(double deltaTime)
    {
        for (auto& [_, entity] : m_Entities)
        {
            if (entity->isActive())
                entity->onUpdate(deltaTime);
        }
    }

    void FScene::onPhysicsUpdate(double deltaTime)
    {
        for (auto& [_, entity] : m_Entities)
        {
            if (entity->isActive())
                entity->onPhysicsUpdate(deltaTime);
        }
    }

    void FScene::onLateUpdate(double deltaTime)
    {
        for (auto& [_, entity] : m_Entities)
        {
            if (entity->isActive())
                entity->onLateUpdate(deltaTime);
        }
    }

    void FScene::onRender(ICommandBuffer* cmdBuffer)
    {
        for (auto& [_, entity] : m_Entities)
        {
            if (entity->isActive())
                entity->onRender(cmdBuffer);
        }
    }
}
