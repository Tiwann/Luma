#pragma once
#include "Camera.h"
#include "Luma/Math/Rect2.h"

namespace Luma
{
    struct FScissor : FRect2u
    {
        FScissor(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
        : FRect2u(x, y, width, height) {};

        FScissor(const FRect2u& rect) : FRect2u(rect){}

        static FScissor fromSize(const uint32_t width, const uint32_t height)
        {
            return FScissor{0, 0, width, height};
        }

        static FScissor fromCamera(const FCamera& camera)
        {
            return FScissor{0, 0, camera.getWidth(), camera.getHeight()};
        }
    };
}