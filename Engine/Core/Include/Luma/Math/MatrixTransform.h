#pragma once
#include "Vector.h"
#include "Matrix.h"

namespace Luma
{
    template<NumberType T>
    struct TQuat;

    template<NumberType T>
    struct TAxisAngle;

    template<NumberType T>
    TMatrix<T, 2, 2> rotate(const TMatrix<T, 2, 2>& m, T angle);

    template<NumberType T>
    TMatrix<T, 3, 3> rotate(const TMatrix<T, 3, 3>& m, const TAxisAngle<T>& axisAngle);

    template<NumberType T>
    TMatrix<T, 4, 4> rotate(const TMatrix<T, 4, 4>& m, const TAxisAngle<T>& axisAngle);

    template<NumberType T>
    TMatrix<T, 3, 3> rotate(const TMatrix<T, 3, 3>& m, const TQuat<T>& quat);

    template<NumberType T>
    TMatrix<T, 4, 4> rotate(const TMatrix<T, 4, 4>& m, const TQuat<T>& quat);

    template<NumberType T>
    TMatrix<T, 2, 2> scale(const TMatrix<T, 2, 2>& m, T scalar);

    template<NumberType T>
    TMatrix<T, 2, 2> scale(const TMatrix<T, 2, 2>& m, const TVector<T, 2>& scale);

    template<NumberType T>
    TMatrix<T, 3, 3> scale(const TMatrix<T, 3, 3>& m, T scalar);

    template<NumberType T>
    TMatrix<T, 3, 3> scale(const TMatrix<T, 3, 3>& m, const TVector<T, 3>& scale);

    template<NumberType T>
    TMatrix<T, 4, 4> scale(const TMatrix<T, 4, 4>& m, T scalar);

    template<NumberType T>
    TMatrix<T, 4, 4> scale(const TMatrix<T, 4, 4>& m, const TVector<T, 3>& scale);

    template<NumberType T>
    TMatrix<T, 3, 3> translate(const TMatrix<T, 3, 3>& m, const TVector<T, 2>& v);

    template<NumberType T>
    TMatrix<T, 4, 4> translate(const TMatrix<T, 4, 4>& m, const TVector<T, 3>& v);
}