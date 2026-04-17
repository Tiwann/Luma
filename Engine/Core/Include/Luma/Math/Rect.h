#pragma once
#include "Concepts.h"

namespace Luma
{
    template<NumberType T, uint32_t N>
    struct TRect;

    using FRect2f = TRect<float, 2>;
    using FRect2d = TRect<double, 2>;
    using FRect2i = TRect<int32_t, 2>;
    using FRect2u = TRect<uint32_t, 2>;

    using FRect3f = TRect<float, 3>;
    using FRect3d = TRect<double, 3>;
    using FRect3i = TRect<int32_t, 3>;
    using FRect3u = TRect<uint32_t, 3>;

    using FRect2D = FRect2f;
    using FRect3D = FRect3f;
}
