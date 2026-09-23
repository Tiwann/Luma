#pragma once
#include "Camera.h"
#include "Luma/Math/Rect2.h"

namespace Luma
{
    struct Scissor : FRect2u
    {
        Scissor(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
        : FRect2u(x, y, width, height) {};

        Scissor(const FRect2u& rect) : FRect2u(rect){}

        static Scissor fromSize(const FVector2u& size)
        {
            return Scissor(0, 0, size.x, size.y);
        }

        static Scissor fromSize(const uint32_t width, const uint32_t height)
        {
            return Scissor{0, 0, width, height};
        }

        static Scissor fromCamera(const Camera& camera)
        {
            return Scissor{0, 0, camera.getWidth(), camera.getHeight()};
        }
    };
}