#pragma once
#include "Vector.h"
#include <limits>
#include <cmath>

namespace Luma
{
    template<NumberType T>
    struct TVector<T, 4>
    {
        union
        {
            struct { T x, y, z, w; };
            T data[4];
        };

        constexpr TVector() : x(T(0)), y(T(0)), z(T(0)), w(T(0)){}
        constexpr TVector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        constexpr TVector(T n) : x(n), y(n), z(n), w(n) {}
        constexpr explicit TVector(const TVector<T, 3>& other, T w = T(1)) : x(other.x), y(other.y), z(other.z), w(w) {}

        const T* valuePtr() const { return data; }
        T*       valuePtr()       { return data; }

        constexpr TVector withX(T x) const { return TVector(x, y, z, w); }
        constexpr TVector withY(T y) const { return TVector(x, y, z, w); }
        constexpr TVector withZ(T z) const { return TVector(x, y, z, w); }
        constexpr TVector withW(T w) const { return TVector(x, y, z, w); }

        constexpr T dot(const TVector& other) const
        {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        constexpr T magnitude() const
        {
            return static_cast<T>(std::sqrt(x * x + y * y + z * z + w * w));
        }

        TVector normalized() const
        {
            const T m = magnitude();
            return m > T(0) ? TVector(x / m, y / m, z / m, w / m) : TVector(T(0));
        }

        TVector apply(T(*function)(T)) const
        {
            if (!function) return *this;
            return TVector(function(x), function(y), function(z), function(w));
        }

        constexpr TVector  operator+(const TVector& other) const { return TVector(x + other.x, y + other.y, z + other.z, w + other.w); }
        constexpr TVector  operator-(const TVector& other) const { return TVector(x - other.x, y - other.y, z - other.z, w - other.w); }
        constexpr TVector  operator-()                    const { return TVector(-x, -y, -z, -w); }
        constexpr TVector  operator*(const TVector& other) const { return TVector(x * other.x, y * other.y, z * other.z, w * other.w); }
        constexpr TVector  operator/(T scalar)            const { return TVector(x / scalar, y / scalar, z / scalar, w / scalar); }

        constexpr TVector& operator+=(const TVector& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
        constexpr TVector& operator-=(const TVector& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
        constexpr TVector& operator*=(T scalar)             { x *= scalar;  y *= scalar;  z *= scalar;  w *= scalar;  return *this; }
        constexpr TVector& operator/=(T scalar)             { x /= scalar;  y /= scalar;  z /= scalar;  w /= scalar;  return *this; }

        constexpr friend TVector operator*(T scalar, const TVector& v)
        {
            return TVector(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
        }

        constexpr bool operator==(const TVector& other) const
        {
            constexpr auto areSame = [&](T a, T b) constexpr {
                constexpr auto abs = [](T n) constexpr { return n > T(0) ? n : -n; };
                return abs(a - b) < std::numeric_limits<T>::epsilon();
            };

            return areSame(x, other.x) && areSame(y, other.y) && areSame(z, other.z) && areSame(w, other.w);
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
        constexpr TVector<U, 4> as() { return TVector<U, 4>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w)); }

        static const TVector Zero;
        static const TVector One;
    };

    template<NumberType T> const TVector<T, 4> TVector<T, 4>::Zero = { T(0), T(0), T(0), T(0) };
    template<NumberType T> const TVector<T, 4> TVector<T, 4>::One  = { T(1), T(1), T(1), T(1) };
}