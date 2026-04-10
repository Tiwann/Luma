#pragma once
#include "Concepts.h"
#include "Vector3.h"

namespace luma
{
    template<NumberType T>
    struct TAxisAngle
    {
        TVector<T, 3> axis;
        T angle;
    };

    using FAxisAngle2f = TAxisAngle<float>;
    using FAxisAngle2d = TAxisAngle<double>;
    using FAxisAngle2u = TAxisAngle<uint32_t>;
    using FAxisAngle2i = TAxisAngle<int32_t>;
}
