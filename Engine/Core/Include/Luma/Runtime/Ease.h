#pragma once
#include "Luma/Math/Concepts.h"
#include <cmath>
#include <numbers>

namespace
{
    template<std::floating_point T>
    constexpr T SQUARE(T t) { return t * t; }

    template<std::floating_point T>
    constexpr T CUBE(T t) { return t * t * t; }

    template<std::floating_point T>
    constexpr T QUART(T t) { return t * t * t * t; }
}

namespace Luma
{
    enum class EEaseType
    {
        Linear,
        QuadraticIn,
        QuadraticOut,
        QuadraticInOut,
        CubicIn,
        CubicOut,
        CubicInOut,
        QuarticIn,
        QuarticOut,
        QuarticInOut,
        ExpoIn,
        ExpoOut,
        ExpoInOut,
        BackIn,
        BackOut,
        BackInOut,
        SineIn,
        SineOut,
        SineInOut,
    };

    template<FloatType T = float>
    using FEaseFunction = T(*)(T);

    template <FloatType T = float>
    static constexpr FEaseFunction<T> getEaseFunction(EEaseType easeType)
    {
        switch (easeType)
        {
        case EEaseType::Linear:
            {
                static constexpr auto f = [](T t) constexpr { return t; };
                return f;
            }
        case EEaseType::QuadraticIn:
            {
                static constexpr auto f = [](T t) constexpr { return SQUARE(t); };
                return f;
            }
        case EEaseType::QuadraticOut:
            {
                static constexpr auto f = [](T t) constexpr{return T(1) - SQUARE(T(1) - t);};
                return f;
            }
        case EEaseType::QuadraticInOut:
            {
                static constexpr auto f = [](T t) constexpr{ return t < 0.5? 2 * SQUARE(t): 1 - SQUARE(-2 * t + 2) / 2; };
                return f;
            }
        case EEaseType::CubicIn:
            {
                static constexpr auto f = [](T t) constexpr { return CUBE(t); };
                return f;
            }
        case EEaseType::CubicOut:
            {
                static constexpr auto f = [](T t) constexpr{return T(1) - CUBE(T(1) - t);};
                return f;
            }
        case EEaseType::CubicInOut:
            {
                static constexpr auto f = [](T t) constexpr { return t < 0.5 ? 4 * CUBE(t) : 1 - CUBE(-2 * t + 2) / 2; };
                return f;
            }
        case EEaseType::QuarticIn:
            {
                static constexpr auto f = [](T t) constexpr { return QUART(t); };
                return f;
            }
        case EEaseType::QuarticOut:
            {
                static constexpr auto f = [](T t) constexpr { return T(1) - QUART(T(1) - t); };
                return f;
            }
        case EEaseType::QuarticInOut:
            {
                static constexpr auto f = [](T t) constexpr { return t < 0.5 ? 8 * QUART(t) : 1 - QUART(-2 * t + 2) / 2; };
                return f;
            }
        case EEaseType::ExpoIn:
            {
                static constexpr auto f = [](T t) constexpr { return t == 0 ? 0 : std::exp2(10 * t - 10); };
                return f;
            }
        case EEaseType::ExpoOut:
            {
                static constexpr auto f = [](T t) constexpr { return t == 1 ? 1 : T(1) - std::exp2(-10 * t); };
                return f;
            }
        case EEaseType::ExpoInOut:
            {
                static constexpr auto f = [](T t) constexpr
                {
                    if (t == 0) return T(0);
                    if (t == 1) return T(1);
                    return t < 0.5
                               ? std::exp2(20 * t - 10) / 2
                               : (2 - std::exp2(-20 * t + 10)) / 2;
                };
                return f;
            }
        case EEaseType::BackIn:
            {
                static constexpr auto f = [](T t) constexpr
                {
                    const T c1 = 1.70158;
                    const T c3 = c1 + 1;
                    return c3 * t * t * t - c1 * t * t;
                };
                return f;
            }
        case EEaseType::BackOut:
            {
                static constexpr auto f = [](T t) constexpr
                {
                    const T c1 = 1.70158;
                    const T c3 = c1 + 1;
                    T u = t - 1;
                    return 1 + c3 * u * u * u + c1 * u * u;
                };
                return f;
            }
        case EEaseType::BackInOut:
            {
                static constexpr auto f = [](T t) constexpr
                {
                    const T c1 = 1.70158;
                    const T c2 = c1 * 1.525;
                    return t < 0.5
                               ? ((2 * t) * (2 * t) * ((c2 + 1) * 2 * t - c2)) / 2
                               : (((2 * t - 2) * (2 * t - 2)) * ((c2 + 1) * (2 * t - 2) + c2) + 2) / 2;
                };
                return f;
            }
        case EEaseType::SineIn:
            {
                static constexpr auto f = [](T t) constexpr
                {
                    return 1 - std::cos((t * std::numbers::pi_v<T>) / 2);
                };
                return f;
            }
        case EEaseType::SineOut:
            {
                static constexpr auto f = [](T t) constexpr
                {
                    return std::sin((t * std::numbers::pi_v<T>) / 2);
                };
                return f;
            }
        case EEaseType::SineInOut:
            {
                static constexpr auto f = [](T t) constexpr
                {
                    return -(std::cos(std::numbers::pi_v<T> * t) - 1) / 2;
                };
                return f;
            }
        default:
            return nullptr;
        }
    }
}
