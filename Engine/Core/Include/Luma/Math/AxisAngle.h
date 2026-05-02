#pragma once
#include "Concepts.h"
#include "Vector3.h"

namespace Luma
{
    template<NumberType T>
    struct TAxisAngle
    {
        TVector<T, 3> axis;
        T angle;
    };

    using FAxisAnglef = TAxisAngle<float>;
    using FAxisAngled = TAxisAngle<double>;
    using FAxisAngleu = TAxisAngle<uint32_t>;
    using FAxisAnglei = TAxisAngle<int32_t>;
}
