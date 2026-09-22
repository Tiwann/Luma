#pragma once
#include "String.h"
#include "HashMap.h"
#include "Luma/Runtime/Hash.h"
#include <cstdint>


namespace Luma
{
    template<typename T>
    struct THasher<TString<T>>
    {
        uint64_t operator()(const TString<T>& str) const
        {
            return FNV1aHash(reinterpret_cast<const uint8_t*>(str.data()), str.count());
        }
    };
}

namespace std
{
    template<>
    struct hash<Luma::FString>
    {
        size_t operator()(const Luma::FString& string) const noexcept
        {
            return Luma::THasher<Luma::FString>()(string);
        }
    };
}
