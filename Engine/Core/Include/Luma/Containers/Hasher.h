#pragma once
#include <functional>

namespace Luma
{
    template<typename T>
    struct FHasher;

    template<typename T>
    struct FHasher : std::hash<T> {};
}
