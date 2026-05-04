#pragma once
#include "Scene.h"
#include "Luma/Containers/Array.h"
#include "Luma/Runtime/Component.h"
#include <cstdint>

namespace Luma
{
    struct FClass;

    class FEntity final : IAsset
    {
    public:
        FEntity(FScene* context) : m_Owner(context) {}

        FEntity* getParent() const;
        void setParent(FEntity* parent);

        const TArray<FEntity*>& getChildren() const;
        void addChild(FEntity* child);
        void removeChild(FEntity* child);

        template<typename T> requires std::is_base_of_v<IComponent, T>
        T* getComponent() const
        {
            for (IComponent* component : m_Components)
                if (T* asT = dynamic_cast<T*>(component))
                    return asT;
            return nullptr;
        }

        template<typename T> requires std::is_base_of_v<IComponent, T>
        T* addComponent()
        {
            T* component = new T();
            component->m_Owner = this;
            m_Components.add(component);
            static_cast<IComponent*>(component)->initialize();
            return component;
        }

        template<typename T> requires std::is_base_of_v<IComponent, T>
        void removeComponent(T* component)
        {
            static_cast<IComponent*>(component)->destroy();
            m_Components.remove(component);
        }

        FScene* getOwner() const;
        EAssetType getAssetType() const override;

        bool isActive() const;
        void setActive(bool active);

    private:
        void initialize();
        void destroy() override;

        void onInit();
        void onDestroy();
        void onUpdate(double deltaTime);
        void onPhysicsUpdate(double deltaTime);
        void onLateUpdate(double deltaTime);
        void onRender(ICommandBuffer* cmdBuffer);

        friend class FScene;
        FScene* m_Owner = nullptr;
        FEntity* m_Parent = nullptr;
        bool m_Active = true;
        TArray<FEntity*> m_Children;
        TArray<IComponent*> m_Components;
    };
}
