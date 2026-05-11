#pragma once
#include "Concepts.h"

namespace Luma
{
    template<NumberType T, uint32_t N>
    struct TRect;

    template<NumberType T>
    using FRect2 = TRect<T, 2>;

    template<NumberType T>
    using FRect3 = TRect<T, 3>;

    using FRect2f = FRect2<float>;
    using FRect2d = FRect2<double>;
    using FRect2i = FRect2<int32_t>;
    using FRect2u = FRect2<uint32_t>;

    using FRect3f = FRect3<float>;
    using FRect3d = FRect3<double>;
    using FRect3i = FRect3<int32_t>;
    using FRect3u = FRect3<uint32_t>;

    using FRect2D = FRect2f;
    using FRect3D = FRect3f;
}
