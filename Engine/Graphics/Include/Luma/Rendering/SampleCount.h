#pragma once
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    enum ESampleCount
    {
        SampleCount1x = BIT(0),
        SampleCount2x = BIT(1),
        SampleCount4x = BIT(2),
        SampleCount8x = BIT(3),
        SampleCount16x = BIT(4),
    };
}
