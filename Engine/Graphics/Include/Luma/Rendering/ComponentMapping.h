#pragma once
#include "ColorChannel.h"
#include "ComponentSwizzle.h"

namespace Luma
{
    struct ComponentMapping
    {
        ComponentSwizzle r = ComponentSwizzle::Identity;
        ComponentSwizzle g = ComponentSwizzle::Identity;
        ComponentSwizzle b = ComponentSwizzle::Identity;
        ComponentSwizzle a = ComponentSwizzle::Identity;

        static ComponentMapping FromColorChannelFlags(ColorChannelFlags channelFlags);
    };
}