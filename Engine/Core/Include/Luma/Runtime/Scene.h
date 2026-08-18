#pragma once
#include "Asset.h"
#include "Luma/Containers/HashMap.h"

namespace Luma
{
    class FEntity;
    class FEntityHandle;
    struct IComponent;
    struct ICommandBuffer;

    class FScene final : public IAsset
    {
    public:
        FScene() = default;

        void initialize();
        void destroy() override;

        void onUpdate(double deltaTime);
        void onPhysicsUpdate(double deltaTime);
        void onLateUpdate(double deltaTime);
        void onRender(ICommandBuffer* cmdBuffer);

        EAssetType getAssetType() const override { return EAssetType::Scene; }

        FEntityHandle createEntity();
        void destroyEntity(FEntityHandle entity);
        TArray<FEntity*> getEntities();

        template<typename T> requires std::is_base_of_v<IComponent, T>
        T* getFirstComponent();

    private:
        friend class FEntity;
        friend class FEntityHandle;

        THashMap<uint32_t, FEntity*> m_Entities;
        static inline uint32_t s_EntityId = 1;
    };
}
