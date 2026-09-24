#pragma once
#include "Luma/Containers/Array.h"
#include "Luma/Memory/Ref.h"

namespace Luma
{
    struct InputProvider;

    class InputSystem
    {
    public:
        template<typename T, typename... Args> requires std::is_base_of_v<InputProvider, T>
        void addProvider(Args&&... args) { m_Providers.emplace(Ref<T>::create(std::forward<Args>(args)...)); }

        bool initialize();
    private:
        Array<Ref<InputProvider>> m_Providers;
    };
}
