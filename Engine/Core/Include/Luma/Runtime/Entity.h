#pragma once
#include "Scene.h"
#include "Luma/Containers/Array.h"
#include "Luma/Runtime/Component.h"
#include <cstdint>

namespace Luma
{
    struct FClass;

    class Entity final : Asset
    {
    public:
        Entity(FScene* context) : m_Owner(context) {}

        Entity* getParent() const;
        void setParent(Entity* parent);

        const Array<Entity*>& getChildren() const;
        void addChild(Entity* child);
        void removeChild(Entity* child);

        template<typename T> requires std::is_base_of_v<Component, T>
        T* getComponent() const
        {
            for (Component* component : m_Components)
                if (T* asT = dynamic_cast<T*>(component))
                    return asT;
            return nullptr;
        }

        template<typename T> requires std::is_base_of_v<Component, T>
        Array<T*> getAllComponentsOfType() const
        {
            Array<T*> components;
            for (Component* component : m_Components)
                if (T* asT = dynamic_cast<T*>(component))
                    components.addUnique(asT);
            return components;
        }

        template<typename T> requires std::is_base_of_v<Component, T>
        T* addComponent()
        {
            T* component = new T();
            component->m_Owner = this;
            m_Components.add(component);
            static_cast<Component*>(component)->initialize();
            return component;
        }

        template<typename T> requires std::is_base_of_v<Component, T>
        void removeComponent(T* component)
        {
            static_cast<Component*>(component)->destroy();
            m_Components.remove(component);
        }

        FScene* getOwner() const;
        AssetType getAssetType() const override;

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
        void onRender(CommandBuffer* cmdBuffer);

        friend class FScene;
        FScene* m_Owner = nullptr;
        Entity* m_Parent = nullptr;
        bool m_Active = true;
        Array<Entity*> m_Children;
        Array<Component*> m_Components;
    };
}
