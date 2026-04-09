#pragma once
#include "Concepts.h"
#include "Vector3.h"
#include "AxisAngle.h"
#include <cmath>

namespace luma
{
    template<Number T>
    struct TQuaternion
    {
        T w{T(1)}, x{T(0)}, y{T(0)}, z{T(0)};

        constexpr TQuaternion() = default;
        constexpr TQuaternion(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {}
        
        constexpr TQuaternion operator+(const TQuaternion& other) const
        {
            return { w + other.w, x + other.x, y + other.y, z + other.z };
        }

        constexpr TQuaternion operator-(const TQuaternion& other) const
        {
            return { w - other.w, x - other.x, y - other.y, z - other.z };
        }

        constexpr TQuaternion& operator+=(const TQuaternion& other)
        {
            w += other.w;
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr TQuaternion& operator-=(const TQuaternion& other)
        {
            w -= other.w;
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        constexpr TQuaternion operator*(const TQuaternion& other) const
        {
            return {
                w * other.w - x * other.x - y * other.y - z * other.z,
                w * other.x + x * other.w + y * other.z - z * other.y,
                w * other.y - x * other.z + y * other.w + z * other.x,
                w * other.z + x * other.y - y * other.x + z * other.w
            };
        }

        constexpr TQuaternion operator*(const T other) const
        {
            return { w * other, x * other, y * other, z * other };
        }

        constexpr TQuaternion& operator*=(const TQuaternion& other)
        {
            *this = operator*(other);
            return *this;
        }

        TQuaternion& operator*=(const T other)
        {
            *this = operator*(other);
            return *this;
        }

        constexpr TQuaternion operator/(const T other) const
        {
            return { w / other, x / other, y / other, z / other };
        }

        constexpr TVector<T, 3> operator*(const TVector<T, 3>& other) const
        {
            const TVector<T, 3> quatVector(x, y, z);
            const TVector<T, 3> uv(quatVector.cross(other));
            const TVector<T, 3> uuv(quatVector.cross(uv));
            return other + (uv * w + uuv) * T(2);
        }

        T magnitude() const
        {
            return std::sqrt(x*x + y*y + z*z + w*w);
        }

        TQuaternion normalize() const
        {
            const T m = magnitude();
            if (m <= T(0)) return TQuaternion();
            return { w / m, y / m, z / m, x / m };
        }

        constexpr TQuaternion conjugated() const
        {
            return { w, -x, -y, -z };
        }

        TQuaternion inverted() const
        {
            const T d = dot(*this);
            const TQuaternion conjugate = conjugated();
            return conjugate / d;
        }

        constexpr T dot(const TQuaternion& other) const
        {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        constexpr TQuaternion cross(const TQuaternion& other) const
        {
            return TQuaternion(
                w * other.w - x * other.x - y * other.y - z * other.z,
                w * other.x + x * other.w + y * other.z - z * other.y,
                w * other.y + y * other.w + z * other.x - x * other.z,
                w * other.z + z * other.w + x * other.y - y * other.x);
        }

        TAxisAngle<T> toAxisAngle() const
        {
            T radians = std::acos(w) * T(2);
            const T sin = std::sin(radians * T(0.5));
            const TVector<T, 3> axis = {x / sin, y / sin, z / sin};
            return TAxisAngle<T>(axis, radians);
        }

        static TQuaternion fromAxisAngle(const TAxisAngle<T>& axisAngle)
        {
            const TVector<T, 3> a = axisAngle.axis.normalized();
            const T halfAngle = axisAngle.angle * T(0.5);
            const T sinHalfAngle = std::sin(halfAngle);

            TQuaternion result;
            result.w = std::cos(halfAngle);
            result.x = a.x * sinHalfAngle;
            result.y = a.y * sinHalfAngle;
            result.z = a.z * sinHalfAngle;
            return result;
        }
        
        static TQuaternion fromEulerAngles(const TVector<T, 3>& eulerAngles)
        {
            const T cosX = std::cos(eulerAngles.x * T(0.5f));
            const T sinX = std::sin(eulerAngles.x * T(0.5f));
            const T cosY = std::cos(eulerAngles.y * T(0.5f));
            const T sinY = std::sin(eulerAngles.y * T(0.5f));
            const T cosZ = std::cos(eulerAngles.z * T(0.5f));
            const T sinZ = std::sin(eulerAngles.z * T(0.5f));

            TQuaternion result;
            result.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
            result.x = sinX * cosY * cosZ - cosX * sinY * sinZ;
            result.y = cosX * sinY * cosZ + sinX * cosY * sinZ;
            result.z = cosX * cosY * sinZ - sinX * sinY * cosZ;
            return result;
        }

        static constexpr TQuaternion lerp(const TQuaternion& a, const TQuaternion& b, double t)
        {
            static constexpr auto lerp = [](T a, T b, double t) constexpr { return a + t * (b - a); };

            return {
                static_cast<T>(lerp(a.w, b.w, t)),
                static_cast<T>(lerp(a.x, b.x, t)),
                static_cast<T>(lerp(a.y, b.y, t)),
                static_cast<T>(lerp(a.z, b.z, t))
            };
        }
    };
}
