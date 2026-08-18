#pragma once
#include "Luma/Math/Rect2.h"

namespace Luma
{
    struct FScissor : FRect2u
    {
        FScissor(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
        : FRect2u(x, y, width, height) {};
    };
}