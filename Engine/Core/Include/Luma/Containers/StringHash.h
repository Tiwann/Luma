#pragma once
#include "String.h"
#include "HashMap.h"
#include <cstdint>


namespace Luma
{
    template<>
    struct THasher<FString>
    {
        static uint64_t hashBytes(const char* data, size_t size)
        {
            uint64_t hash = 1469598103934665603ull;
            for (size_t i = 0; i < size; i++)
            {
                hash ^= (uint8_t)data[i];
                hash *= 1099511628211ull;
            }

            return hash;
        }

        uint64_t operator()(const FString& str) const
        {
            return hashBytes(str.data(), str.size());
        }
    };
}
