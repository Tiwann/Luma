#pragma once
#include "Concepts.h"
#include "Vector.h"
#include <numbers>
#include <limits>

namespace Luma
{
    template<FloatType F>
    static constexpr F pi = std::numbers::pi_v<F>;

    template<FloatType F>
    static constexpr F epsilon = std::numeric_limits<F>::epsilon();

    template<FloatType F>
    constexpr F radians(F degrees)
    {
        return degrees * pi<F> / F(180.0);
    }

    template<FloatType F>
    constexpr F degrees(F radians)
    {
        return radians *  F(180.0) / pi<F>;
    }

    template<NumberType N>
    constexpr N abs(N n) { return n > 0 ? n : -n; }

    template<FloatType F>
    constexpr bool areEqual(F a, F b)
    {
        return abs(a - b) <= epsilon<F>();
    }

    template<FloatType F>
    constexpr bool isZero(F value)
    {
        return areEqual(value, F(0.0));
    }

    template<NumberType T>
    constexpr T max(T a, T b)
    {
        return a > b ? a : b;
    }

    template<NumberType T>
    constexpr T max(T a, T b, T c)
    {
        return max(max(a, b), c);
    }

    template<NumberType T>
    constexpr T min(T a, T b)
    {
        return a < b ? a : b;
    }

    template<NumberType T>
    constexpr T min(T a, T b, T c)
    {
        return min(min(a, b), c);
    }

    template<NumberType T, uint32_t N>
    constexpr TVector<T, N> getMipDim(uint32_t mipLevel, const TVector<T, N>& dim)
    {
        return TVector<T, N>{ max(T(1), dim.x >> mipLevel), max(T(1), dim.y >> mipLevel), max(T(1), dim.z >> mipLevel) };
    }
}