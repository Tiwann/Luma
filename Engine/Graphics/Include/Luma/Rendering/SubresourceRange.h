#pragma once
#include <cstdint>

namespace luma
{
    struct FSubresourceRange
    {
        uint32_t baseMipLevel = 0;
        uint32_t mipCount = 0;
        uint32_t baseArrayLayer = 0;
        uint32_t layerCount = 0;
    };
}
