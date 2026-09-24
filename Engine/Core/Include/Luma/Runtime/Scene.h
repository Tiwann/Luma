#pragma once
#include "Asset.h"
#include "Luma/Containers/HashMap.h"

namespace Luma
{
    class Entity;
    class FEntityHandle;
    struct Component;
    struct CommandBuffer;

    class FScene final : public Asset
    {
    public:
        FScene() = default;

        void initialize();
        void destroy() override;

        void onUpdate(double deltaTime);
        void onPhysicsUpdate(double deltaTime);
        void onLateUpdate(double deltaTime);
        void onRender(CommandBuffer* cmdBuffer);

        AssetType getAssetType() const override { return AssetType::Scene; }

        FEntityHandle createEntity();
        void destroyEntity(FEntityHandle entity);
        Array<Entity*> getEntities();

        template<typename T> requires std::is_base_of_v<Component, T>
        T* getFirstComponent();

    private:
        friend class Entity;
        friend class FEntityHandle;

        HashMap<uint32_t, Entity*> m_Entities;
        static inline uint32_t s_EntityId = 1;
    };
}
