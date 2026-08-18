#pragma once
#include "Luma/Containers/Array.h"
#include "Luma/Memory/Ref.h"

namespace Luma
{
    struct IInputProvider;

    class FInputSystem
    {
    public:
        template<typename T, typename... Args> requires std::is_base_of_v<IInputProvider, T>
        void addProvider(Args&&... args) { m_Providers.emplace(Ref<T>::create(std::forward<Args>(args)...)); }

        bool initialize();
    private:
        TArray<Ref<IInputProvider>> m_Providers;
    };
}
