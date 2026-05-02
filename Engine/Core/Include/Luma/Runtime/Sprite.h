#pragma once
#include "Luma/Memory/Ref.h"
#include <cstdint>

namespace Luma
{
    struct ITexture;
    
    struct Sprite
    {
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        WeakRef<ITexture> texture = nullptr;

        bool operator==(const Sprite& other) const
        {
            return x == other.x && y == other.y && width == other.width && height == other.height && texture == other.texture;
        }

        static Sprite fromTexture(WeakRef<ITexture> texture);
    };
}
