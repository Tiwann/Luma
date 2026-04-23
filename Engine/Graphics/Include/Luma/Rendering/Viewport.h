#pragma once
#include "Luma/Math/Rect2.h"

namespace Luma
{
    struct FViewport
    {
        FRect2f area;
        float minDepth = 0, maxDepth = 0;
    };
}
