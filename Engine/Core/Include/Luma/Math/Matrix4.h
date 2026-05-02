#pragma once
#include "Vector4.h"
#include "Matrix.h"
#include <cmath>

namespace Luma
{
    template<NumberType T>
    struct TQuat;

    template<NumberType T>
    struct TMatrix<T, 4, 4>
    {
        using Vector  = TVector<T, 4>;
        using Vector3 = TVector<T, 3>;

        union
        {
            struct
            {
                T m00, m10, m20, m30,
                  m01, m11, m21, m31,
                  m02, m12, m22, m32,
                  m03, m13, m23, m33;
            };
            Vector columns[4];
        };

        constexpr TMatrix() : m00{T(1)}, m10{T(0)}, m20{T(0)}, m30{T(0)},
                              m01{T(0)}, m11{T(1)}, m21{T(0)}, m31{T(0)},
                              m02{T(0)}, m12{T(0)}, m22{T(1)}, m32{T(0)},
                              m03{T(0)}, m13{T(0)}, m23{T(0)}, m33{T(1)} {}

        constexpr TMatrix(
            T m00, T m10, T m20, T m30,
            T m01, T m11, T m21, T m31,
            T m02, T m12, T m22, T m32,
            T m03, T m13, T m23, T m33)
            : m00(m00), m10(m10), m20(m20), m30(m30),
              m01(m01), m11(m11), m21(m21), m31(m31),
              m02(m02), m12(m12), m22(m22), m32(m32),
              m03(m03), m13(m13), m23(m23), m33(m33) {}

        constexpr explicit TMatrix(
            const Vector& c0, const Vector& c1,
            const Vector& c2, const Vector& c3)
        {
            columns[0] = c0;
            columns[1] = c1;
            columns[2] = c2;
            columns[3] = c3;
        }

        explicit TMatrix(const TMatrix<T, 3, 3>& mat)
        {
            columns[0] = TVector<T, 4>(mat[0], T(0));
            columns[1] = TVector<T, 4>(mat[1], T(0));
            columns[2] = TVector<T, 4>(mat[2], T(0));
            columns[3] = TVector<T, 4>(T(0), T(0), T(0), T(1));
        }


        const T* valuePtr() const { return &m00; }
        T*       valuePtr()       { return &m00; }

        T magnitude() const
        {
            return static_cast<T>(std::sqrt(
                m00*m00 + m10*m10 + m20*m20 + m30*m30 +
                m01*m01 + m11*m11 + m21*m21 + m31*m31 +
                m02*m02 + m12*m12 + m22*m22 + m32*m32 +
                m03*m03 + m13*m13 + m23*m23 + m33*m33));
        }

        constexpr T determinant() const
        {
            const T m00_ = m11*(m22*m33 - m23*m32) - m12*(m21*m33 - m23*m31) + m13*(m21*m32 - m22*m31);
            const T m01_ = m10*(m22*m33 - m23*m32) - m12*(m20*m33 - m23*m30) + m13*(m20*m32 - m22*m30);
            const T m02_ = m10*(m21*m33 - m23*m31) - m11*(m20*m33 - m23*m30) + m13*(m20*m31 - m21*m30);
            const T m03_ = m10*(m21*m32 - m22*m31) - m11*(m20*m32 - m22*m30) + m12*(m20*m31 - m21*m30);

            return m00 * m00_ - m01 * m01_ + m02 * m02_ - m03 * m03_;
        }

        constexpr TMatrix inverted() const
        {
            const T c00 =   m11*(m22*m33 - m23*m32) - m12*(m21*m33 - m23*m31) + m13*(m21*m32 - m22*m31);
            const T c01 = -(m10*(m22*m33 - m23*m32) - m12*(m20*m33 - m23*m30) + m13*(m20*m32 - m22*m30));
            const T c02 =   m10*(m21*m33 - m23*m31) - m11*(m20*m33 - m23*m30) + m13*(m20*m31 - m21*m30);
            const T c03 = -(m10*(m21*m32 - m22*m31) - m11*(m20*m32 - m22*m30) + m12*(m20*m31 - m21*m30));

            const T c10 = -(m01*(m22*m33 - m23*m32) - m02*(m21*m33 - m23*m31) + m03*(m21*m32 - m22*m31));
            const T c11 =   m00*(m22*m33 - m23*m32) - m02*(m20*m33 - m23*m30) + m03*(m20*m32 - m22*m30);
            const T c12 = -(m00*(m21*m33 - m23*m31) - m01*(m20*m33 - m23*m30) + m03*(m20*m31 - m21*m30));
            const T c13 =   m00*(m21*m32 - m22*m31) - m01*(m20*m32 - m22*m30) + m02*(m20*m31 - m21*m30);

            const T c20 =   m01*(m12*m33 - m13*m32) - m02*(m11*m33 - m13*m31) + m03*(m11*m32 - m12*m31);
            const T c21 = -(m00*(m12*m33 - m13*m32) - m02*(m10*m33 - m13*m30) + m03*(m10*m32 - m12*m30));
            const T c22 =   m00*(m11*m33 - m13*m31) - m01*(m10*m33 - m13*m30) + m03*(m10*m31 - m11*m30);
            const T c23 = -(m00*(m11*m32 - m12*m31) - m01*(m10*m32 - m12*m30) + m02*(m10*m31 - m11*m30));

            const T c30 = -(m01*(m12*m23 - m13*m22) - m02*(m11*m23 - m13*m21) + m03*(m11*m22 - m12*m21));
            const T c31 =   m00*(m12*m23 - m13*m22) - m02*(m10*m23 - m13*m20) + m03*(m10*m22 - m12*m20);
            const T c32 = -(m00*(m11*m23 - m13*m21) - m01*(m10*m23 - m13*m20) + m03*(m10*m21 - m11*m20));
            const T c33 =   m00*(m11*m22 - m12*m21) - m01*(m10*m22 - m12*m20) + m02*(m10*m21 - m11*m20);

            const T invDet = T(1) / (m00*c00 + m01*c10 + m02*c20 + m03*c30);

            return TMatrix
            {
                c00*invDet, c01*invDet, c02*invDet, c03*invDet,   // col 0
                c10*invDet, c11*invDet, c12*invDet, c13*invDet,   // col 1
                c20*invDet, c21*invDet, c22*invDet, c23*invDet,   // col 2
                c30*invDet, c31*invDet, c32*invDet, c33*invDet    // col 3
            };
        }

        constexpr TMatrix transposed() const
        {
            return TMatrix(
                m00, m01, m02, m03,
                m10, m11, m12, m13,
                m20, m21, m22, m23,
                m30, m31, m32, m33
            );
        }

        constexpr Vector getRow(uint32_t i) const
        {
            switch (i)
            {
            case 0: return { m00, m01, m02, m03 };
            case 1: return { m10, m11, m12, m13 };
            case 2: return { m20, m21, m22, m23 };
            case 3: return { m30, m31, m32, m33 };
            default: throw;
            }
        }

        constexpr Vector getCol(uint32_t i) const
        {
            return columns[i];
        }

        const Vector& operator[](uint32_t i) const
        {
            return columns[i];
        }

        Vector& operator[](uint32_t i)
        {
            return columns[i];
        }

        constexpr Vector operator*(const Vector& v) const
        {
            const Vector r0 = getRow(0);
            const Vector r1 = getRow(1);
            const Vector r2 = getRow(2);
            const Vector r3 = getRow(3);
            return { r0.dot(v), r1.dot(v), r2.dot(v), r3.dot(v) };
        }

        constexpr TMatrix operator*(const TMatrix& other) const
        {
            const Vector r0 = getRow(0);
            const Vector r1 = getRow(1);
            const Vector r2 = getRow(2);
            const Vector r3 = getRow(3);

            const Vector c0 = other.getCol(0);
            const Vector c1 = other.getCol(1);
            const Vector c2 = other.getCol(2);
            const Vector c3 = other.getCol(3);

            return TMatrix(
                r0.dot(c0), r1.dot(c0), r2.dot(c0), r3.dot(c0),
                r0.dot(c1), r1.dot(c1), r2.dot(c1), r3.dot(c1),
                r0.dot(c2), r1.dot(c2), r2.dot(c2), r3.dot(c2),
                r0.dot(c3), r1.dot(c3), r2.dot(c3), r3.dot(c3)
            );
        }

        static constexpr bool isSquare() { return true; }

        static const TMatrix Identity;
    };

    template <NumberType T>
    const TMatrix<T, 4, 4> TMatrix<T, 4, 4>::Identity = TMatrix();
}
