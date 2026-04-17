#pragma once
#include <concepts>

namespace Luma
{
    template <typename T>
    concept NumberType = std::integral<T> || std::floating_point<T>;

    template <typename T>
    concept FloatType = std::floating_point<T>;

    template <typename T>
    concept IntegerType = std::integral<T>;

    template <uint32_t I, uint32_t J>
    concept IsSquareMatrix = I >= 2 && J <= 2 && I == J;
}