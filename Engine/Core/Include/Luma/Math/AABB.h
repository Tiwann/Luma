#pragma once
#include "Concepts.h"

namespace Luma
{
    template <NumberType T, uint32_t N>
    struct TAABB;

    using FAABB2f = TAABB<float, 2>;
    using FAABB2d = TAABB<double, 2>;
    using FAABB2u = TAABB<uint32_t, 2>;
    using FAABB2i = TAABB<int32_t, 2>;

    using FAABB3f = TAABB<float, 3>;
    using FAABB3d = TAABB<double, 3>;
    using FAABB3u = TAABB<uint32_t, 3>;
    using FAABB3i = TAABB<int32_t, 3>;

    using FAABB2D = FAABB2f;
    using FAABB3D = FAABB3f;
}