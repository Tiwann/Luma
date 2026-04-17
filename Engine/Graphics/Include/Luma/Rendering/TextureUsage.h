#pragma once
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    enum class ETextureUsageBits
    {
        None = 0,
        Sampled = BIT(0),
        Storage = BIT(1),
        ColorAttachment = BIT(3),
        DepthStencilAttachment = BIT(4),
    };

    using FTextureUsageFlags = Flags<ETextureUsageBits>;
}