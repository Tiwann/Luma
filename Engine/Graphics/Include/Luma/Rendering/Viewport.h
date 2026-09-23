#pragma once
#include "Camera.h"
#include "Luma/Math/Rect2.h"

namespace Luma
{
    struct Viewport
    {
        float x = 0, y = 0, width = 0, height = 0;
        float minDepth = 0, maxDepth = 0;

        Viewport(float x, float y, float w, float h, float minDepth = 0.0f, float maxDepth = 1.0f)
            : x(x), y(y), width(w), height(h), minDepth(minDepth), maxDepth(maxDepth){}

        explicit Viewport(const FRect2f& bounds, float minDepth = 0.0f, float maxDepth = 1.0f);

        static Viewport fromSize(const FVector2f& size)
        {
            return Viewport{0.0f, 0.0f, size.x, size.y};
        }

        static Viewport fromSize(float width, float height)
        {
            return Viewport{0.0f, 0.0f, width, height};
        }

        static Viewport fromCamera(const Camera& camera)
        {
            return Viewport{0.0f, 0.0f, static_cast<float>(camera.getWidth()), static_cast<float>(camera.getHeight())};
        }
;    };
}
