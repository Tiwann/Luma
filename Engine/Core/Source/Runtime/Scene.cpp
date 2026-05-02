#include "Luma/Runtime/Scene.h"
#include "Luma/Runtime/Entity.h"
#include "Luma/Runtime/Component.h"

namespace Luma
{
    FScene::FScene()
    {

    }

    FEntity FScene::createEntity()
    {
        const uint32_t handle = static_cast<uint32_t>(m_Registry.create());
        return FEntity(handle, this);
    }

    void FScene::destroyEntity(FEntity entity)
    {
        LUMA_ASSERT(entity.m_Context == this, "Entity context don't match!");
        m_Registry.destroy(static_cast<entt::entity>(entity.m_Handle));
        const auto view = m_Registry.view<IComponent>();
        view.each([](IComponent& component) { component.destroy(); });
    }

    void FScene::destroy()
    {
        const auto view = m_Registry.view<entt::entity>();
        view.each([this](const entt::entity& handle)
        {
            FEntity entity(static_cast<uint32_t>(handle), this);
            destroyEntity(entity);
        });
    }

    void FScene::onInit()
    {
        const auto view = m_Registry.view<IComponent>();
        view.each([](IComponent& component)
        {
            component.onInit();
        });
    }

    void FScene::onDestroy()
    {
    }

    void FScene::onUpdate(double deltaTime)
    {
        const auto view = m_Registry.view<IComponent>();
        view.each([&deltaTime](IComponent& component)
        {
            component.onUpdate(deltaTime);
        });
    }

    void FScene::onPhysicsUpdate(double deltaTime)
    {
    }

    void FScene::onRender(ICommandBuffer* cmdBuffer, const FCamera& camera)
    {
        const auto view = m_Registry.view<IComponent>();
        view.each([cmdBuffer, camera](IComponent& component)
        {
            component.onRender(cmdBuffer, camera);
        });
    }
}
