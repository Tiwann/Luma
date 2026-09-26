#pragma once
#include "Luma/Math/Concepts.h"
#include "Luma/Containers/String.h"
#include <cstdint>

namespace Luma
{
    uint64_t FNV1aHash(const uint8_t* data, uint64_t dataSize);

    template<typename Key>
    struct Hasher
    {
        uint64_t operator()(const Key& key) const
        {
            return FNV1aHash(reinterpret_cast<const uint8_t*>(&key), sizeof(Key));
        }
    };

    template<IntegerType T>
    struct Hasher<T>
    {
        uint64_t operator()(const T& key) const
        {
            return static_cast<uint64_t>(key);
        }
    };

    template<Character T>
    struct Hasher<StringBase<T>>
    {
        uint64_t operator()(const StringBase<T>& str) const
        {
            return FNV1aHash(reinterpret_cast<const uint8_t*>(str.data()), str.count());
        }
    };
}
