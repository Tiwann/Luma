#pragma once
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    enum class TextureUsage
    {
        None = 0,
        Sampled = BIT(0),
        Storage = BIT(1),
        Color = BIT(3),
        DepthStencil = BIT(4),
        RenderTarget = BIT(5)
    };

    using TextureUsageFlags = Flags<TextureUsage>;
}