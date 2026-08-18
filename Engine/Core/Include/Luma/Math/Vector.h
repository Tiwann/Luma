#pragma once
#include "Concepts.h"
#include "Luma/Runtime/Types.h"

namespace Luma
{
    template<NumberType T, uint32_t N> requires(N >= 2 && N <= 4)
    struct TVector;

    template<NumberType T>
    using FVector2 = TVector<T, 2>;

    template<NumberType T>
    using FVector3 = TVector<T, 3>;

    template<NumberType T>
    using FVector4 = TVector<T, 4>;

    using FVector2f = FVector2<float>;
    using FVector2d = FVector2<double>;
    using FVector2u = FVector2<uint32_t>;
    using FVector2i = FVector2<int32_t>;

    using FVector3f = FVector3<float>;
    using FVector3d = FVector3<double>;
    using FVector3u = FVector3<uint32_t>;
    using FVector3i = FVector3<int32_t>;

    using FVector4f = FVector4<float>;
    using FVector4d = FVector4<double>;
    using FVector4u = FVector4<uint32_t>;
    using FVector4i = FVector4<int32_t>;

    using FVector2D = FVector2f;
    using FVector3D = FVector3f;
    using FVector4D = FVector4f;
}