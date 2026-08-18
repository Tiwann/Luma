#pragma once
#include "Luma/Runtime/Scene.h"
#include "Luma/Runtime/Entity.h"
#include "Luma/Runtime/Component.h"

namespace Luma
{
    template <typename T> requires std::is_base_of_v<IComponent, T>
    T* FScene::getFirstComponent()
    {
        for (auto& [_, entity] : m_Entities)
        {
            T* component = entity->getComponent<T>();
            if (component) return component;
        }

        return nullptr;
    }
}