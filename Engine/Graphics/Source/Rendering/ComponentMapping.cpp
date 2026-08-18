#include "Luma/Rendering/ComponentMapping.h"

namespace Luma
{
    FComponentMapping FComponentMapping::FromColorChannelFlags(const FColorChannelFlags channelFlags)
    {
        FComponentMapping swizzle;
        swizzle.r = channelFlags & EColorChannelBits::Red ? EComponentSwizzle::Identity : EComponentSwizzle::Zero;
        swizzle.g = channelFlags & EColorChannelBits::Green ? EComponentSwizzle::Identity : EComponentSwizzle::Zero;
        swizzle.b = channelFlags & EColorChannelBits::Blue ? EComponentSwizzle::Identity : EComponentSwizzle::Zero;
        swizzle.a = channelFlags & EColorChannelBits::Alpha ? EComponentSwizzle::Identity : EComponentSwizzle::Zero;
        return swizzle;
    }
}
