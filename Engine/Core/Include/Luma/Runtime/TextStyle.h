#pragma once
#include "Flags.h"

namespace Luma
{
    enum class ETextStyleBits
    {
        None = 0,
        Bold = BIT(0),
        Italic = BIT(1),
        Underline = BIT(2),
    };

    using FTextStyleFlags = TFlags<ETextStyleBits>;
}