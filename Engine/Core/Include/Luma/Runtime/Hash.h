#pragma once
#include <cstdint>

namespace Luma
{
    uint64_t FNV1aHash(const uint8_t* data, uint64_t dataSize);
}
