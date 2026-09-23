#pragma once
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    enum class TextureAspect
    {
        None = 0,
        Color = BIT(0),
        Depth = BIT(1),
        Stencil = BIT(2),
    };

    using TextureAspectFlags = TFlags<TextureAspect>;
}
