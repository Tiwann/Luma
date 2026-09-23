#pragma once
#include <cstdint>

namespace Luma
{
    struct TextureSubresourceRange
    {
        uint32_t startMipIndex = 0;
        uint32_t mipCount = 0;
        uint32_t startArrayIndex = 0;
        uint32_t layerCount = 0;
    };
}
