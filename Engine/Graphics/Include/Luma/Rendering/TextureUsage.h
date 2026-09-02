#pragma once
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    enum class ETextureUsageBits
    {
        None = 0,
        Sampled = BIT(0),
        Storage = BIT(1),
        ColorTarget = BIT(3),
        DepthStencilTarget = BIT(4),
    };

    using FTextureUsageFlags = TFlags<ETextureUsageBits>;
}