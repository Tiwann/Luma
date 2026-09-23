#include "Luma/Rendering/ComponentMapping.h"

namespace Luma
{
    ComponentMapping ComponentMapping::FromColorChannelFlags(const ColorChannelFlags channelFlags)
    {
        ComponentMapping swizzle;
        swizzle.r = channelFlags & ColorChannels::Red ? ComponentSwizzle::Identity : ComponentSwizzle::Zero;
        swizzle.g = channelFlags & ColorChannels::Green ? ComponentSwizzle::Identity : ComponentSwizzle::Zero;
        swizzle.b = channelFlags & ColorChannels::Blue ? ComponentSwizzle::Identity : ComponentSwizzle::Zero;
        swizzle.a = channelFlags & ColorChannels::Alpha ? ComponentSwizzle::Identity : ComponentSwizzle::Zero;
        return swizzle;
    }
}
