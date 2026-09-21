#pragma once
#include "Camera.h"
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

        static FViewport fromSize(float width, float height)
        {
            return FViewport{0.0f, 0.0f, width, height};
        }

        static FViewport fromCamera(const FCamera& camera)
        {
            return FViewport{0.0f, 0.0f, static_cast<float>(camera.getWidth()), static_cast<float>(camera.getHeight())};
        }
;    };
}
