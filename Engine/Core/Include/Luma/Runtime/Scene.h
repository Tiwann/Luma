#pragma once
#include "Asset.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Rendering/Camera.h"
#include <entt/entt.hpp>


namespace Luma
{
    class FEntity;
    struct ICommandBuffer;

    class FScene : public IAsset, public IRefCounted<FScene>
    {
    public:
        FScene();
        EAssetType getAssetType() const override { return EAssetType::Scene; }

        FEntity createEntity();
        void destroyEntity(FEntity entity);

        void destroy();

        void onInit();
        void onDestroy();

        void onUpdate(double deltaTime);
        void onPhysicsUpdate(double deltaTime);
        void onRender(ICommandBuffer* cmdBuffer, const FCamera& camera);
    private:
        friend class FEntity;
        entt::registry m_Registry;
    };
}
