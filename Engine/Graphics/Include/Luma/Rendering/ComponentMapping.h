#pragma once
#include "ColorChannel.h"
#include "ComponentSwizzle.h"

namespace Luma
{
    struct FComponentMapping
    {
        EComponentSwizzle r = EComponentSwizzle::Identity;
        EComponentSwizzle g = EComponentSwizzle::Identity;
        EComponentSwizzle b = EComponentSwizzle::Identity;
        EComponentSwizzle a = EComponentSwizzle::Identity;

        static FComponentMapping FromColorChannelFlags(FColorChannelFlags channelFlags);
    };
}