#pragma once
#include "Vector.h"
#include "Matrix.h"

namespace luma
{
    template<Number T>
    struct TMatrix<T, 2, 2>
    {
        using VectorType = TVector<T, 2>;

        union
        {
            struct { T m00{T(1)}, m10{T(0)}, m01{T(0)}, m11{T(1)}; };
            VectorType columns[2];
        };

        constexpr TMatrix() : m00(T(1)), m10(T(0)), m01(T(0)), m11(T(1)){}
        constexpr TMatrix(T m00, T m10, T m01, T m11) : m00(m00), m10(m10), m01(m01), m11(m11){}
        constexpr explicit TMatrix(const VectorType& col1, const VectorType& col2)
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
                VectorType(m11 * invDet, -m10 * invDet),
                VectorType(-m01 * invDet, m00 * invDet),
            };;
        }

        constexpr TMatrix transposed() const
        {
            return TMatrix(
                VectorType(m00, m10),
                VectorType(m01, m11)
            );
        }

        constexpr VectorType getRow(uint32_t i) const
        {
            switch (i)
            {
            case 0: return { m00, m01 };
            case 1: return { m10, m11 };
            default: throw;
            }
        }

        constexpr VectorType getCol(uint32_t i) const
        {
            switch (i)
            {
            case 0: return { m00, m10 };
            case 1: return { m01, m11 };
            default: throw;
            }
        }

        constexpr VectorType operator*(const VectorType& other) const
        {
            const VectorType row0 = getRow(0);
            const VectorType row1 = getRow(1);
            const VectorType result = { row0.dot(other), row1.dot(other) };
            return result;
        }

        constexpr TMatrix operator*(const TMatrix& other) const
        {
            const VectorType row0 = getRow(0);
            const VectorType row1 = getRow(1);

            const VectorType col0 = other.getCol(0);
            const VectorType col1 = other.getCol(1);

            const VectorType newCol0 { row0.dot(col0), row1.dot(col0) };
            const VectorType newCol1 { row0.dot(col1), row1.dot(col1) };
            return TMatrix { newCol0, newCol1 };
        }

        static constexpr bool isSquare() { return true; }
    };
}