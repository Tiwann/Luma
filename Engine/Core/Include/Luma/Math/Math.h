#pragma once
#include "Concepts.h"
#include <cmath>

#define SQUARE(t) ((t) * (t))
#define CUBE(t) ((t) * (t) * (t))
#define QUART(t) ((t) * (t) * (t) * (t))

    
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

    struct FMath
    {
        template<FloatType T = float>
        static constexpr T Pi = T(3.141592653589793);

        template<FloatType T = float>
        static constexpr T Tau = T(2) * Pi<T>;

        template<FloatType T = float>
        static constexpr T Deg2Rad = Pi<T> / T(180);

        template<FloatType T = float>
        static constexpr T Rad2Deg = T(180) / Pi<T>;

        template<NumberType T>
        static constexpr T clamp(T x, T min, T max) { return x < min ? min : (x > max ? max : x); }

        template<NumberType T>
        static constexpr T saturate(T x) { return clamp<T>(x, T(0), T(1)); }

        template<FloatType T>
        static T cos(T x) { return std::cos(x); }

        template<FloatType T>
        static T sin(T x) { return std::sin(x); }

        template<FloatType T>
        static T lerp(T a, T b, T t) { return a + (b - a) * t; }

        template <FloatType T = float>
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
                    static constexpr auto f = [](T t) constexpr { return T(1) - SQUARE(T(1) - t); };
                    return f;
                }
            case EEaseType::QuadraticInOut:
                {
                    static constexpr auto f = [](T t) constexpr
                    {
                        return t < 0.5 ? 2 * SQUARE(t) : 1 - SQUARE(-2 * t + 2) / 2;
                    };
                    return f;
                }
            case EEaseType::CubicIn:
                {
                    static constexpr auto f = [](T t) constexpr { return CUBE(t); };
                    return f;
                }
            case EEaseType::CubicOut:
                {
                    static constexpr auto f = [](T t) constexpr { return T(1) - CUBE(T(1) - t); };
                    return f;
                }
            case EEaseType::CubicInOut:
                {
                    static constexpr auto f = [](T t) constexpr
                    {
                        return t < 0.5 ? 4 * CUBE(t) : 1 - CUBE(-2 * t + 2) / 2;
                    };
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
                    static constexpr auto f = [](T t) constexpr
                    {
                        return t < 0.5 ? 8 * QUART(t) : 1 - QUART(-2 * t + 2) / 2;
                    };
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
                        return 1 - cos((t * Pi<T>) / 2);
                    };
                    return f;
                }
            case EEaseType::SineOut:
                {
                    static constexpr auto f = [](T t) constexpr
                    {
                        return std::sin((t * Pi<T>) / 2);
                    };
                    return f;
                }
            case EEaseType::SineInOut:
                {
                    static constexpr auto f = [](T t) constexpr
                    {
                        return -(std::cos(Pi<T> * t) - 1) / 2;
                    };
                    return f;
                }
            default:
                return nullptr;
            }
        }
        
        template<EEaseType E, FloatType T>
        static constexpr T ease(T value) { return getEaseFunction<T>(E)(value); }
    };
}