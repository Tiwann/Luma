#pragma once
#include "Vector.h"
#include <limits>
#include <cmath>

namespace Luma
{
    template<NumberType T>
    struct TVector<T, 2>
    {
        union
        {
            struct { T x{0}, y{0}; };
            T data[2];
        };

        constexpr TVector() = default;
        constexpr TVector(T x, T y) : x(x), y(y) {}
        constexpr TVector(T n) : x(n), y(n) {}
        constexpr explicit TVector(const TVector<T, 3>& other) : x(other.x), y(other.y) {}
        constexpr explicit TVector(const TVector<T, 4>& other) : x(other.x), y(other.y) {}


        const T* valuePtr() const { return data; }
        T* valuePtr() { return data; }

        constexpr TVector withX(T x) const
        {
            return TVector(x, y);
        }

        constexpr TVector withY(T y) const
        {
            return TVector(x, y);
        }

        constexpr T dot(const TVector& other) const
        {
            return x * other.x + y * other.y;
        }

        T magnitude() const
        {
            return static_cast<T>(std::sqrt(static_cast<double>(x * x + y * y)));
        }

        TVector normalized() const
        {
            const double m = magnitude();
            return m > 0.0 ? TVector(x / m, y / m) : TVector(T(0), T(0));
        }

        TVector apply(T(*function)(T)) const
        {
            if (!function) return *this;
            return TVector(function(x), function(y));
        }

        constexpr TVector operator+(const TVector& other) const
        {
            return TVector(x + other.x, y + other.y);
        }

        constexpr TVector operator-(const TVector& other) const
        {
            return TVector(x - other.x, y - other.y);
        }

        constexpr TVector operator-() const
        {
            return TVector(-x, -y);
        }

        constexpr TVector& operator+=(const TVector& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        constexpr TVector& operator-=(const TVector& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        constexpr TVector operator*(const TVector& other) const
        {
            return TVector(x * other.x, y * other.y);
        }

        constexpr friend TVector operator*(T scalar, const TVector& vector)
        {
            return TVector(vector.x * scalar, vector.y * scalar);
        }

        constexpr TVector operator/(T scalar) const
        {
            return TVector(x / scalar, y / scalar);
        }

        constexpr TVector& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        constexpr TVector& operator/=(T scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        constexpr bool operator==(const TVector& other) const
        {
            static constexpr auto areEqual = [](T x, T y) constexpr
            {
                static constexpr auto abs = [](T n) constexpr { return n > 0 ? n : -n; };
                return abs(x - y) < std::numeric_limits<T>::epsilon();
            };

            return areEqual(x, other.x) && areEqual(y, other.y);
        }

        T& operator[](uint32_t i)
        {
            return data[i];
        }

        const T& operator[](uint32_t i) const
        {
            return data[i];
        }

        static T angle(const TVector& a, const TVector& b)
        {
            return std::acos(a.dot(b) / (a.magnitude() * b.magnitude()));
        }

        static constexpr TVector lerp(const TVector& a, const TVector& b, double t)
        {
            return a + t * (b - a);
        }

        static constexpr TVector quadraticBezier(const TVector& a, const TVector& b, const TVector& c, double t)
        {
            const TVector p0 = lerp(a, b, t);
            const TVector p1 = lerp(b, c, t);
            return lerp(p0, p1, t);
        }

        static constexpr TVector cubicBezier(const TVector& a, const TVector& b, const TVector& c, const TVector& d, double t)
        {
            const TVector p0 = quadraticBezier(a, b, c, t);
            const TVector p1 = quadraticBezier(b, c, d, t);
            return lerp(p0, p1, t);
        }

        template<NumberType U>
        constexpr TVector<U, 2> as() { return TVector<U, 2>(static_cast<U>(x), static_cast<U>(y)); }

        static const TVector Zero;
        static const TVector One;
        static const TVector Right;
        static const TVector Up;
        static const TVector Left;
        static const TVector Down;
    };

    template<NumberType T> const TVector<T, 2> TVector<T, 2>::Zero  = { T(0), T(0) };
    template<NumberType T> const TVector<T, 2> TVector<T, 2>::One   = { T(1), T(1) };
    template<NumberType T> const TVector<T, 2> TVector<T, 2>::Right = { T(1), T(0) };
    template<NumberType T> const TVector<T, 2> TVector<T, 2>::Up    = { T(0), T(1) };
    template<NumberType T> const TVector<T, 2> TVector<T, 2>::Left  = { T(-1), T(0) };
    template<NumberType T> const TVector<T, 2> TVector<T, 2>::Down  = { T(0), T(-1) };
}
