#pragma once
#include "ColorChannel.h"
#include "ComponentSwizzle.h"

namespace luma
{
    struct FComponentMapping
    {
        EComponentSwizzle r = EComponentSwizzle::Identity;
        EComponentSwizzle g = EComponentSwizzle::Identity;
        EComponentSwizzle b = EComponentSwizzle::Identity;
        EComponentSwizzle a = EComponentSwizzle::Identity;

        static const FComponentMapping Identity;

        static FComponentMapping FromColorChannelFlags(ColorChannelFlags channelFlags);
    };
}