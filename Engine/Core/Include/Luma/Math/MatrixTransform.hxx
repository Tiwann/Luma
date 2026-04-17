#pragma once
#include "Luma/Math/MatrixTransform.h"
#include "Luma/Math/MathMacros.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Vector4.h"
#include "Luma/Math/Matrix2.h"
#include "Luma/Math/Matrix3.h"
#include "Luma/Math/Matrix4.h"
#include "Luma/Math/Quaternion.h"
#include <cmath>

namespace Luma
{
    template <NumberType T>
    TMatrix<T, 2, 2> rotate(const TMatrix<T, 2, 2>& m, T angle)
    {
        const T c = std::cos(angle);
        const T s = std::sin(angle);
        const TMatrix<T, 2, 2> rotation { c, s, -s, c };
        return rotation * m;
    }

    template<NumberType T>
    TMatrix<T, 3, 3> rotate(const TMatrix<T, 3, 3>& m, const TAxisAngle<T>& axisAngle)
    {
        const TVector<T, 3> axis = axisAngle.axis.normalized();
        const T c = std::cos(axisAngle.angle);
        const T s = std::sin(axisAngle.angle);
        const T invC = T(1.0) - c;
        const T xx = LUMA_SQR(axis.x);
        const T yy = LUMA_SQR(axis.y);
        const T zz = LUMA_SQR(axis.z);
        const T xy = axis.x * axis.y;
        const T xz = axis.x * axis.z;
        const T yz = axis.y * axis.z;
        const T x = axis.x;
        const T y = axis.y;
        const T z = axis.z;

        const TVector<T, 3> col1 = TVector<T, 3>(xx * invC + c, xy * invC + z * s, xz * invC - y * s);
        const TVector<T, 3> col2 = TVector<T, 3>(xy * invC - z * s, yy * invC + c, yz * invC + x * s);
        const TVector<T, 3> col3 = TVector<T, 3>(xz * invC + y * s, yz * invC - x * s, zz * invC + c);

        const TMatrix<T, 3, 3> rotation {col1, col2, col3};
        return rotation * m;
    }

    template<NumberType T>
    TMatrix<T, 4, 4> rotate(const TMatrix<T, 4, 4>& m, const TAxisAngle<T>& axisAngle)
    {
        const TVector<T, 3> axis = axisAngle.axis.normalized();
        const T c = std::cos(axisAngle.angle);
        const T s = std::sin(axisAngle.angle);
        const T invC = T(1.0) - c;
        const T xx = LUMA_SQR(axis.x);
        const T yy = LUMA_SQR(axis.y);
        const T zz = LUMA_SQR(axis.z);
        const T xy = axis.x * axis.y;
        const T xz = axis.x * axis.z;
        const T yz = axis.y * axis.z;
        const T x = axis.x;
        const T y = axis.y;
        const T z = axis.z;

        const TVector<T, 4> col1 = TVector<T, 4>(xx * invC + c, xy * invC + z * s, xz * invC - y * s, T(0));
        const TVector<T, 4> col2 = TVector<T, 4>(xy * invC - z * s, yy * invC + c, yz * invC + x * s, T(0));
        const TVector<T, 4> col3 = TVector<T, 4>(xz * invC + y * s, yz * invC - x * s, zz * invC + c, T(0));
        const TVector<T, 4> col4 = TVector<T, 4>(T(0), T(0), T(0), T(1));

        const TMatrix<T, 4, 4> rotation {col1, col2, col3, col4};
        return rotation * m;
    }

    template<NumberType T>
    TMatrix<T, 3, 3> rotate(const TMatrix<T, 3, 3>& m, const TQuat<T>& quat)
    {
        return quat.toMatrix3() * m;
    }

    template<NumberType T>
    TMatrix<T, 4, 4> rotate(const TMatrix<T, 4, 4>& m, const TQuat<T>& quat)
    {
        return quat.toMatrix4() * m;
    }

    template<NumberType T>
    TMatrix<T, 2, 2> scale(const TMatrix<T, 2, 2>& m, T scalar)
    {
        const TMatrix<T, 2, 2> scaleMatrix { scalar, T(0), T(0), scalar };
        return scaleMatrix * m;
    }

    template<NumberType T>
    TMatrix<T, 2, 2> scale(const TMatrix<T, 2, 2>& m, const TVector<T, 2>& scale)
    {
        const TMatrix<T, 2, 2> scaleMatrix { scale.x, T(0), T(0), scale.y };
        return scaleMatrix * m;
    }

    template<NumberType T>
    TMatrix<T, 3, 3> scale(const TMatrix<T, 3, 3>& m, T scalar)
    {
        const TMatrix<T, 3, 3> scaleMatrix
        {
            scalar, T(0), T(0),
            T(0), scalar, T(0),
            T(0), T(0), scalar
        };

        return scaleMatrix * m;
    }

    template<NumberType T>
    TMatrix<T, 3, 3> scale(const TMatrix<T, 3, 3>& m, const TVector<T, 3>& scale)
    {
        const TMatrix<T, 3, 3> scaleMatrix
        {
            scale.x, T(0), T(0),
            T(0), scale.y, T(0),
            T(0), T(0), scale.z
        };

        return scaleMatrix * m;
    }

    template<NumberType T>
    TMatrix<T, 4, 4> scale(const TMatrix<T, 4, 4>& m, T scalar)
    {
        const TMatrix<T, 4, 4> scaleMatrix
        {
            scalar, T(0), T(0), T(0),
            T(0), scalar, T(0), T(0),
            T(0), T(0), scalar, T(0),
            T(0), T(0), T(0), T(1)
        };
        return scaleMatrix * m;
    }

    template<NumberType T>
    TMatrix<T, 4, 4> scale(const TMatrix<T, 4, 4>& m, const TVector<T, 3>& scale)
    {
        const TMatrix<T, 4, 4> scaleMatrix
        {
            scale.x, T(0), T(0), T(0),
            T(0), scale.y, T(0), T(0),
            T(0), T(0), scale.z, T(0),
            T(0), T(0), T(0), T(1)
        };
        return scaleMatrix * m;
    }

    template<NumberType T>
    TMatrix<T, 3, 3> translate(const TMatrix<T, 3, 3>& m, const TVector<T, 2>&  v)
    {
        const TMatrix<T, 3, 3> translate
        {
            T(1), T(0), T(0),
            T(0), T(1), T(0),
            v.x, v.y, T(1),
        };
        return translate * m;
    }

    template<NumberType T>
    TMatrix<T, 4, 4> translate(const TMatrix<T, 4, 4>& m, const TVector<T, 3>& v)
    {
        const TMatrix<T, 4, 4> translate
        {
            T(1), T(0), T(0), T(0),
            T(0), T(1), T(0), T(0),
            T(0), T(0), T(1), T(0),
            v.x, v.y, v.z, T(1)
        };
        return translate * m;
    }

    template TMatrix<float, 2, 2> rotate(const TMatrix<float, 2, 2>&, float);
    template TMatrix<float, 3, 3> rotate(const TMatrix<float, 3, 3>&, const TAxisAngle<float>&);
    template TMatrix<float, 4, 4> rotate(const TMatrix<float, 4, 4>&, const TAxisAngle<float>&);
    template TMatrix<float, 3, 3> rotate(const TMatrix<float, 3, 3>&, const TQuat<float>&);
    template TMatrix<float, 4, 4> rotate(const TMatrix<float, 4, 4>&, const TQuat<float>&);
    template TMatrix<float, 2, 2> scale(const TMatrix<float, 2, 2>&, float);
    template TMatrix<float, 2, 2> scale(const TMatrix<float, 2, 2>&, const TVector<float, 2>&);
    template TMatrix<float, 3, 3> scale(const TMatrix<float, 3, 3>&, float);
    template TMatrix<float, 3, 3> scale(const TMatrix<float, 3, 3>&, const TVector<float, 3>&);
    template TMatrix<float, 4, 4> scale(const TMatrix<float, 4, 4>&, float);
    template TMatrix<float, 4, 4> scale(const TMatrix<float, 4, 4>&, const TVector<float, 3>&);
    template TMatrix<float, 3, 3> translate(const TMatrix<float, 3, 3>&, const TVector<float, 2>&);
    template TMatrix<float, 4, 4> translate(const TMatrix<float, 4, 4>&, const TVector<float, 3>&);

    template TMatrix<double, 2, 2> rotate(const TMatrix<double, 2, 2>&, double);
    template TMatrix<double, 3, 3> rotate(const TMatrix<double, 3, 3>&, const TAxisAngle<double>&);
    template TMatrix<double, 4, 4> rotate(const TMatrix<double, 4, 4>&, const TAxisAngle<double>&);
    template TMatrix<double, 3, 3> rotate(const TMatrix<double, 3, 3>&, const TQuat<double>&);
    template TMatrix<double, 4, 4> rotate(const TMatrix<double, 4, 4>&, const TQuat<double>&);
    template TMatrix<double, 2, 2> scale(const TMatrix<double, 2, 2>&, double);
    template TMatrix<double, 2, 2> scale(const TMatrix<double, 2, 2>&, const TVector<double, 2>&);
    template TMatrix<double, 3, 3> scale(const TMatrix<double, 3, 3>&, double);
    template TMatrix<double, 3, 3> scale(const TMatrix<double, 3, 3>&, const TVector<double, 3>&);
    template TMatrix<double, 4, 4> scale(const TMatrix<double, 4, 4>&, double);
    template TMatrix<double, 4, 4> scale(const TMatrix<double, 4, 4>&, const TVector<double, 3>&);
    template TMatrix<double, 3, 3> translate(const TMatrix<double, 3, 3>&, const TVector<double, 2>&);
    template TMatrix<double, 4, 4> translate(const TMatrix<double, 4, 4>&, const TVector<double, 3>&);
}