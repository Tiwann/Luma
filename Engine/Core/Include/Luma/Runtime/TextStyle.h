#pragma once
#include "Flags.h"

namespace Luma
{
    enum class TextStyle
    {
        Regular = 0,
        Bold = BIT(0),
        Italic = BIT(1),
        Underline = BIT(2),
    };

    using FTextStyleFlags = Flags<TextStyle>;
}