#pragma once
#include "Scene.h"
#include "Luma/Containers/Array.h"
#include "Luma/Containers/HashMap.h"
#include <cstdint>

namespace Luma
{
    struct IComponent;
    struct FClass;

    class FEntity
    {
    public:
        FEntity() = default;
        FEntity(uint32_t handle, FScene* context);
        FEntity(decltype(nullptr)){}

        FEntity getParent() const;
        void setParent(FEntity parent);

        TArray<FEntity> getChildren() const;
        void addChild(FEntity child);
        void removeChild(FEntity child);

        bool operator==(const FEntity& other) const;

        template<typename T>
        T* getComponent() const
        {
            return m_Context->m_Registry.try_get<T>(m_Handle);
        }

        template<typename T>
        T* addComponent()
        {
            auto& component = m_Context->m_Registry.emplace<T>(static_cast<entt::entity>(m_Handle));
            return &component;
        }

        template<typename T>
        void removeComponent(T* component)
        {
            LUMA_ASSERT(component, "Invalid component!");
            m_Context->m_Registry.remove(component);
        }
    private:
        FScene* m_Context = nullptr;
        uint32_t m_Handle = 0;
        uint32_t m_ParentHandle = 0;
        TArray<uint32_t> m_ChildrenHandles;
        bool m_BeingDestroyed = false;

        friend struct THasher<FEntity>;
        friend class FScene;
    };

    template<>
    struct THasher<FEntity>
    {
        static void hashCombine(uint64_t& seed, const uint64_t value)
        {
            seed ^= value + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
        }

        uint64_t operator()(const FEntity& entity) const
        {
            uint64_t seed = 0;
            hashCombine(seed, reinterpret_cast<uint64_t>(entity.m_Context));
            hashCombine(seed, entity.m_Handle);
            return seed;
        }
    };
}
