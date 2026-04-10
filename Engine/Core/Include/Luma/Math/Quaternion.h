#pragma once
#include "Concepts.h"
#include "AxisAngle.h"
#include "Vector.h"
#include "Vector.h"
#include "Matrix.h"
#include <cmath>

namespace luma
{
    template<NumberType T>
    struct TQuat
    {
        T w{T(1)}, x{T(0)}, y{T(0)}, z{T(0)};

        constexpr TQuat() = default;
        constexpr TQuat(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {}
        
        constexpr TQuat operator+(const TQuat& other) const
        {
            return { w + other.w, x + other.x, y + other.y, z + other.z };
        }

        constexpr TQuat operator-(const TQuat& other) const
        {
            return { w - other.w, x - other.x, y - other.y, z - other.z };
        }

        constexpr TQuat& operator+=(const TQuat& other)
        {
            w += other.w;
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr TQuat& operator-=(const TQuat& other)
        {
            w -= other.w;
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        constexpr TQuat operator*(const TQuat& other) const
        {
            return {
                w * other.w - x * other.x - y * other.y - z * other.z,
                w * other.x + x * other.w + y * other.z - z * other.y,
                w * other.y - x * other.z + y * other.w + z * other.x,
                w * other.z + x * other.y - y * other.x + z * other.w
            };
        }

        constexpr TQuat operator*(const T other) const
        {
            return { w * other, x * other, y * other, z * other };
        }

        constexpr TQuat& operator*=(const TQuat& other)
        {
            *this = operator*(other);
            return *this;
        }

        TQuat& operator*=(const T other)
        {
            *this = operator*(other);
            return *this;
        }

        constexpr TQuat operator/(const T other) const
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

        TQuat normalize() const
        {
            const T m = magnitude();
            if (m <= T(0)) return TQuat();
            return { w / m, y / m, z / m, x / m };
        }

        constexpr TQuat conjugated() const
        {
            return { w, -x, -y, -z };
        }

        TQuat inverted() const
        {
            const T d = dot(*this);
            const TQuat conjugate = conjugated();
            return conjugate / d;
        }

        constexpr T dot(const TQuat& other) const
        {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        constexpr TQuat cross(const TQuat& other) const
        {
            return TQuat(
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

        TMatrix<T, 4, 4> toMatrix4() const
        {
            const T xx = x * x;
            const T yy = y * y;
            const T zz = z * z;
            const T xy = x * y;
            const T xz = x * z;
            const T yz = y * z;
            const T wx = w * x;
            const T wy = w * y;
            const T wz = w * z;

            TMatrix<T, 4, 4> result = TMatrix<T, 4, 4>::Identity;
            result[0][0] = T(1.0) - T(2.0) * (yy + zz);
            result[0][1] = T(2.0) * (xy + wz);
            result[0][2] = T(2.0) * (xz - wy);
            result[1][0] = T(2.0) * (xy - wz);
            result[1][1] = T(1.0) - T(2.0) * (xx + zz);
            result[1][2] = T(2.0) * (yz + wx);
            result[2][0] = T(2.0) * (xz + wy);
            result[2][1] = T(2.0) * (yz - wx);
            result[2][2] = T(1.0) - T(2.0) * (xx + yy);
            return result;
        }

        TMatrix<T, 3, 3> toMatrix3() const
        {
            const T xx = x * x;
            const T yy = y * y;
            const T zz = z * z;
            const T xy = x * y;
            const T xz = x * z;
            const T yz = y * z;
            const T wx = w * x;
            const T wy = w * y;
            const T wz = w * z;

            TMatrix<T, 3, 3> result = TMatrix<T, 3, 3>::Identity;
            result[0][0] = T(1.0) - T(2.0) * (yy + zz);
            result[0][1] = T(2.0) * (xy + wz);
            result[0][2] = T(2.0) * (xz - wy);
            result[1][0] = T(2.0) * (xy - wz);
            result[1][1] = T(1.0) - T(2.0) * (xx + zz);
            result[1][2] = T(2.0) * (yz + wx);
            result[2][0] = T(2.0) * (xz + wy);
            result[2][1] = T(2.0) * (yz - wx);
            result[2][2] = T(1.0) - T(2.0) * (xx + yy);
            return result;
        }

        static TQuat fromAxisAngle(const TAxisAngle<T>& axisAngle)
        {
            const TVector<T, 3> a = axisAngle.axis.normalized();
            const T halfAngle = axisAngle.angle * T(0.5);
            const T sinHalfAngle = std::sin(halfAngle);

            TQuat result;
            result.w = std::cos(halfAngle);
            result.x = a.x * sinHalfAngle;
            result.y = a.y * sinHalfAngle;
            result.z = a.z * sinHalfAngle;
            return result;
        }
        
        static TQuat fromEulerAngles(const TVector<T, 3>& eulerAngles)
        {
            const T cosX = std::cos(eulerAngles.x * T(0.5f));
            const T sinX = std::sin(eulerAngles.x * T(0.5f));
            const T cosY = std::cos(eulerAngles.y * T(0.5f));
            const T sinY = std::sin(eulerAngles.y * T(0.5f));
            const T cosZ = std::cos(eulerAngles.z * T(0.5f));
            const T sinZ = std::sin(eulerAngles.z * T(0.5f));

            TQuat result;
            result.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
            result.x = sinX * cosY * cosZ - cosX * sinY * sinZ;
            result.y = cosX * sinY * cosZ + sinX * cosY * sinZ;
            result.z = cosX * cosY * sinZ - sinX * sinY * cosZ;
            return result;
        }

        static constexpr TQuat lerp(const TQuat& a, const TQuat& b, double t)
        {
            static constexpr auto lerp = [](T a, T b, double t) constexpr { return a + t * (b - a); };

            return {
                static_cast<T>(lerp(a.w, b.w, t)),
                static_cast<T>(lerp(a.x, b.x, t)),
                static_cast<T>(lerp(a.y, b.y, t)),
                static_cast<T>(lerp(a.z, b.z, t))
            };
        }

        static const TQuat Identity;
    };

    template<NumberType T>
    const TQuat<T> TQuat<T>::Identity = TQuat{T(1), T(0), T(0), T(0)};

    using FQuatf = TQuat<float>;
    using FQuatd = TQuat<double>;
    using FQuat = FQuatf;
}
