#pragma once
#include "Vector.h"
#include <limits>
#include <cmath>

namespace Luma
{
    template<NumberType T>
    struct TVector<T, 3>
    {
        union
        {
            struct { T x, y, z; };
            T data[3];
        };

        constexpr TVector() : x(T(0)), y(T(0)), z(T(0)){}
        constexpr TVector(T x, T y, T z) : x(x), y(y), z(z) {}
        constexpr TVector(T n) : x(n), y(n), z(n) {}
        constexpr TVector(const TVector<T, 2>& v, T z) : x(v.x), y(v.y), z(z) {}
        constexpr explicit TVector(const TVector<T, 4>& other) : x(other.x), y(other.y), z(other.z) {}


        const T* valuePtr() const { return data; }
        T* valuePtr() { return data; }

        constexpr TVector withX(T x) const
        {
            return TVector(x, y, z);
        }
        
        constexpr TVector withY(T y) const
        {
            return TVector(x, y, z);
        }
        
        constexpr TVector withZ(T z) const
        {
            return TVector(x, y, z);
        }

        constexpr T dot(const TVector& other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }
        
        constexpr TVector cross(const TVector& other) const
        {
            return {
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            };
        }

        constexpr T magnitude() const
        {
            return static_cast<T>(std::sqrt(static_cast<double>(x * x + y * y + z * z)));
        }

        TVector normalized() const
        {
            const double m = magnitude();
            return m > 0.0 ? TVector(x / m, y / m, z / m) : TVector(T(0), T(0), T(0));
        }

        TVector apply(T(*function)(T))
        {
            if (!function) return *this;
            return TVector(function(x), function(y), function(z));
        }

        constexpr TVector operator+(const TVector& other) const
        {
            return TVector(x + other.x, y + other.y, z + other.z);
        }

        constexpr TVector operator-(const TVector& other) const
        {
            return TVector(x - other.x, y - other.y, z - other.z);
        }

        constexpr TVector operator-() const
        {
            return TVector(-x, -y, -z);
        }

        constexpr TVector& operator+=(const TVector& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr TVector& operator-=(const TVector& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        constexpr TVector operator*(const TVector& other) const
        {
            return TVector(x * other.x, y * other.y, z * other.z);
        }

        constexpr friend TVector operator*(T scalar, const TVector& vector)
        {
            return TVector(vector.x * scalar, vector.y * scalar, vector.z * scalar);
        }

        constexpr TVector operator/(T scalar) const
        {
            return TVector(x / scalar, y / scalar, z / scalar);
        }

        constexpr TVector& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        constexpr TVector& operator*=(const TVector& vector)
        {
            x *= vector.x;
            y *= vector.y;
            z *= vector.z;
            return *this;
        }

        constexpr TVector& operator/=(T scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        constexpr bool operator==(const TVector& other) const
        {
            static constexpr auto areEqual = [](T x, T y) constexpr
            {
                static constexpr auto abs = [](T n) constexpr { return n > 0 ? n : -n; };
                return abs(x - y) < std::numeric_limits<T>::epsilon();
            };

            return areEqual(x, other.x) && areEqual(y, other.y) && areEqual(z, other.z);
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
        constexpr TVector<U, 3> as() { return TVector<U, 3>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)); }

        static const TVector Zero;
        static const TVector One;
        static const TVector Right;
        static const TVector Up;
        static const TVector Left;
        static const TVector Down;
        static const TVector Forward;
        static const TVector Backward;
    };

    template<NumberType T> const TVector<T, 3> TVector<T, 3>::Zero      = { T(0), T(0), T(0) };
    template<NumberType T> const TVector<T, 3> TVector<T, 3>::One       = { T(1), T(1), T(1) };
    template<NumberType T> const TVector<T, 3> TVector<T, 3>::Right     = { T(1), T(0), T(0) };
    template<NumberType T> const TVector<T, 3> TVector<T, 3>::Up        = { T(0), T(1), T(0) };
    template<NumberType T> const TVector<T, 3> TVector<T, 3>::Left      = { T(-1), T(0), T(0) };
    template<NumberType T> const TVector<T, 3> TVector<T, 3>::Down      = { T(0), T(-1), T(0) };
    template<NumberType T> const TVector<T, 3> TVector<T, 3>::Forward   = { T(0), T(0), T(1) };
    template<NumberType T> const TVector<T, 3> TVector<T, 3>::Backward  = { T(0), T(0), T(-1) };
}
