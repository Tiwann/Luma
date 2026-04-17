#pragma once
#include "Concepts.h"
#include "Vector.h"

namespace Luma
{
    template<NumberType T, uint32_t N, typename Vector = TVector<T, N>>
    struct TRay
    {
        Vector position;
        Vector direction;
    };

    using FRay2f = TRay<float, 2>;
    using FRay2d = TRay<double, 2>;
    using FRay2u = TRay<uint32_t, 2>;
    using FRay2i = TRay<int32_t, 2>;

    using FRay3f = TRay<float, 3>;
    using FRay3d = TRay<double, 3>;
    using FRay3u = TRay<uint32_t, 3>;
    using FRay3i = TRay<int32_t, 3>;

    using FRay2D = FRay2f;
    using FRay3D = FRay3f;
}
