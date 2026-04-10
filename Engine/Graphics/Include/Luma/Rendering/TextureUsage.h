#pragma once
#include "Runtime/Flags.h"

namespace luma
{
    enum class ETextureUsageFlagBits
    {
        None = 0,
        Sampled = BIT(0),
        Storage = BIT(1),
        Transfer = BIT(2),
        ColorAttachment = BIT(3),
        DepthStencilAttachment = BIT(4),
        Default = Sampled | Transfer
    };

    using TextureUsageFlags = Flags<TextureUsageFlagBits>;
}