#pragma once
#include "Vector.h"
#include "Matrix.h"
#include <cmath>

namespace Luma
{
    template<NumberType T>
    struct TMatrix<T, 2, 2>
    {
        using Vector = TVector<T, 2>;

        union
        {
            struct { T m00{T(1)}, m10{T(0)},
                       m01{T(0)}, m11{T(1)}; };
            Vector columns[2];
        };

        constexpr TMatrix() : m00(T(1)), m10(T(0)), m01(T(0)), m11(T(1)){}
        constexpr TMatrix(T m00, T m10, T m01, T m11) : m00(m00), m10(m10), m01(m01), m11(m11){}
        constexpr explicit TMatrix(const Vector& col1, const Vector& col2)
        {
            m00 = col1.x;
            m10 = col1.y;
            m01 = col2.x;
            m11 = col2.y;
        }

        const T* valuePtr() const { return &m00; }
        T* valuePtr() { return &m00; }

        T magnitude() const
        {
            return static_cast<T>(std::sqrt(m00*m00 + m01*m01 + m10*m10 + m11*m11));
        }

        constexpr T determinant() const
        {
            return m00 * m11 - m01 * m10;
        }

        constexpr TMatrix inverted() const
        {
            T det = determinant();
            T invDet = T(1.0) / det;
            return TMatrix
            {
                Vector(m11 * invDet, -m10 * invDet),
                Vector(-m01 * invDet, m00 * invDet),
            };
        }

        constexpr TMatrix transposed() const
        {
            return TMatrix(
                Vector(m00, m01),
                Vector(m10, m11)
            );
        }

        constexpr Vector getRow(uint32_t i) const
        {
            switch (i)
            {
            case 0: return { m00, m01 };
            case 1: return { m10, m11 };
            default: throw;
            }
        }

        constexpr Vector getCol(uint32_t i) const
        {
            switch (i)
            {
            case 0: return { m00, m10 };
            case 1: return { m01, m11 };
            default: throw;
            }
        }

        const Vector& operator[](uint32_t i) const
        {
            return columns[i];
        }

        Vector& operator[](uint32_t i)
        {
            return columns[i];
        }

        constexpr Vector operator*(const Vector& other) const
        {
            const Vector row0 = getRow(0);
            const Vector row1 = getRow(1);
            const Vector result = { row0.dot(other), row1.dot(other) };
            return result;
        }

        constexpr TMatrix operator*(const TMatrix& other) const
        {
            const Vector row0 = getRow(0);
            const Vector row1 = getRow(1);

            const Vector col0 = other.getCol(0);
            const Vector col1 = other.getCol(1);

            const Vector newCol0 { row0.dot(col0), row1.dot(col0) };
            const Vector newCol1 { row0.dot(col1), row1.dot(col1) };
            return TMatrix { newCol0, newCol1 };
        }

        static constexpr bool isSquare() { return true; }

        static const TMatrix Identity;
    };

    template <NumberType T>
    const TMatrix<T, 2, 2> TMatrix<T, 2, 2>::Identity = TMatrix();
}