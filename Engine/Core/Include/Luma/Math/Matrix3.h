#pragma once
#include "Vector.h"
#include "Matrix.h"
#include <cmath>

namespace Luma
{
    template<NumberType T>
    struct TMatrix<T, 3, 3>
    {
        using Vector = TVector<T, 3>;

        union
        {
            struct
            {
                T m00{T(1)}, m10{T(0)}, m20{T(0)},
                  m01{T(0)}, m11{T(1)}, m21{T(0)},
                  m02{T(0)}, m12{T(0)}, m22{T(1)};
            };
            Vector columns[3];
        };

        constexpr TMatrix() = default;

        constexpr TMatrix(
            T m00, T m10, T m20,
            T m01, T m11, T m21,
            T m02, T m12, T m22)
            : m00(m00), m10(m10), m20(m20),
              m01(m01), m11(m11), m21(m21),
              m02(m02), m12(m12), m22(m22) {}

        constexpr explicit TMatrix(const Vector& c0, const Vector& c1, const Vector& c2)
        {
            columns[0] = c0;
            columns[1] = c1;
            columns[2] = c2;
        }

        const T* valuePtr() const { return &m00; }
        T* valuePtr() { return &m00; }

        constexpr T determinant() const
        {
            return
                m00 * (m11 * m22 - m21 * m12) -
                m01 * (m10 * m22 - m20 * m12) +
                m02 * (m10 * m21 - m20 * m11);
        }

        constexpr TMatrix transposed() const
        {
            return TMatrix(
                m00, m01, m02,
                m10, m11, m12,
                m20, m21, m22
            );
        }

        TMatrix inverted() const
        {
            const float invDet = T(1.0) / determinant();

            TMatrix inv;
            inv[0][0] = + (columns[1][1] * columns[2][2] - columns[2][1] * columns[1][2]) * invDet;
            inv[1][0] = - (columns[1][0] * columns[2][2] - columns[2][0] * columns[1][2]) * invDet;
            inv[2][0] = + (columns[1][0] * columns[2][1] - columns[2][0] * columns[1][1]) * invDet;
            inv[0][1] = - (columns[0][1] * columns[2][2] - columns[2][1] * columns[0][2]) * invDet;
            inv[1][1] = + (columns[0][0] * columns[2][2] - columns[2][0] * columns[0][2]) * invDet;
            inv[2][1] = - (columns[0][0] * columns[2][1] - columns[2][0] * columns[0][1]) * invDet;
            inv[0][2] = + (columns[0][1] * columns[1][2] - columns[1][1] * columns[0][2]) * invDet;
            inv[1][2] = - (columns[0][0] * columns[1][2] - columns[1][0] * columns[0][2]) * invDet;
            inv[2][2] = + (columns[0][0] * columns[1][1] - columns[1][0] * columns[0][1]) * invDet;
            return inv;
        }

        constexpr Vector getRow(uint32_t i) const
        {
            switch (i)
            {
            case 0: return { m00, m01, m02 };
            case 1: return { m10, m11, m12 };
            case 2: return { m20, m21, m22 };
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
            return { r0.dot(v), r1.dot(v), r2.dot(v) };
        }

        constexpr TMatrix operator*(const TMatrix& other) const
        {
            const Vector r0 = getRow(0);
            const Vector r1 = getRow(1);
            const Vector r2 = getRow(2);

            const Vector c0 = other.getCol(0);
            const Vector c1 = other.getCol(1);
            const Vector c2 = other.getCol(2);

            return TMatrix(
                r0.dot(c0), r1.dot(c0), r2.dot(c0),
                r0.dot(c1), r1.dot(c1), r2.dot(c1),
                r0.dot(c2), r1.dot(c2), r2.dot(c2)
            );
        }

        static constexpr bool isSquare() { return true; }

        static const TMatrix Identity;
    };

    template <NumberType T>
    const TMatrix<T, 3, 3> TMatrix<T, 3, 3>::Identity = TMatrix();
}