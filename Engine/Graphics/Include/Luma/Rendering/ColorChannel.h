#pragma once
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    enum class EColorChannelBits
    {
        None = 0,
        Red = BIT(0),
        Green = BIT(1),
        Blue = BIT(2),
        Alpha = BIT(3),
    };

    typedef Flags<EColorChannelBits> FColorChannelFlags;
}
