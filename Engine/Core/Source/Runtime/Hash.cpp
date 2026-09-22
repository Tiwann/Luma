#include "Luma/Runtime/Hash.h"

namespace Luma
{
    static constexpr uint64_t FNV_offset_basis = 0xcbf29ce484222325;
    static constexpr uint64_t FNV_prime = 0x100000001b3;

    uint64_t FNV1aHash(const uint8_t* data, uint64_t dataSize)
    {
        uint64_t hash = FNV_offset_basis;
        for (uint32_t i = 0; i < dataSize; i++)
        {
            hash ^= data[i];
            hash *= FNV_prime;
        }

        return hash;
    }
}
