#pragma once
#include "Luma/Math/Rect2.h"

namespace Luma
{
    struct FViewport
    {
        float x = 0, y = 0, width = 0, height = 0;
        float minDepth = 0, maxDepth = 0;

        FViewport(float x, float y, float w, float h, float minDepth = 0.0f, float maxDepth = 1.0f)
            : x(x), y(y), width(w), height(h), minDepth(minDepth), maxDepth(maxDepth){}

        explicit FViewport(const FRect2f& bounds, float minDepth = 0.0f, float maxDepth = 1.0f);
    };
}
