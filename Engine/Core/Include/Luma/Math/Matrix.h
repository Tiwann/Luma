#pragma once
#include "Concepts.h"
#include "Vector.h"
#include "AxisAngle.h"
#include <cmath>

namespace Luma
{
    template<NumberType T, uint32_t I, uint32_t J> requires(I >= 2 && J >= 2)
    struct TMatrix;

    template<NumberType T>
    using FMatrix2 = TMatrix<T, 2, 2>;

    template<NumberType T>
    using FMatrix3 = TMatrix<T, 3, 3>;

    template<NumberType T>
    using FMatrix4 = TMatrix<T, 4, 4>;

    using FMatrix2f = FMatrix2<float>;
    using FMatrix2d = FMatrix2<double>;
    using FMatrix2u = FMatrix2<uint32_t>;
    using FMatrix2i = FMatrix2<int32_t>;

    using FMatrix3f = FMatrix3<float>;
    using FMatrix3d = FMatrix3<double>;
    using FMatrix3u = FMatrix3<uint32_t>;
    using FMatrix3i = FMatrix3<int32_t>;

    using FMatrix4f = FMatrix4<float>;
    using FMatrix4d = FMatrix4<double>;
    using FMatrix4u = FMatrix4<uint32_t>;
    using FMatrix4i = FMatrix4<int32_t>;
}