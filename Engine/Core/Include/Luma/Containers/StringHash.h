#pragma once
#include "Character.h"
#include "String.h"
#include "HashMap.h"
#include "Luma/Runtime/Hash.h"
#include <cstdint>


namespace Luma
{
    template<Character T>
    struct Hasher<StringBase<T>>
    {
        uint64_t operator()(const StringBase<T>& str) const
        {
            return FNV1aHash(reinterpret_cast<const uint8_t*>(str.data()), str.count());
        }
    };
}

namespace std
{
    using Luma::Character;
    using Luma::StringBase;
    using Luma::Hasher;

    template<Character T>
    struct hash<StringBase<T>>
    {
        size_t operator()(const StringBase<T>& string) const noexcept
        {
            return Hasher<StringBase<T>>()(string);
        }
    };
}
