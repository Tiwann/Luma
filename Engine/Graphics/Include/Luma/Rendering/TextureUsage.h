#pragma once
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    enum class TextureUsage
    {
        None = 0,
        Sampled = BIT(0),
        Storage = BIT(1),
        ColorTarget = BIT(3),
        DepthStencilTarget = BIT(4),
    };

    using TextureUsageFlags = TFlags<TextureUsage>;
}