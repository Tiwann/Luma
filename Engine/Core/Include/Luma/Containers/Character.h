#pragma once
#include <type_traits>

namespace Luma
{
    template<typename T>
    struct TIsCharacter : std::false_type {};

    template<>
    struct TIsCharacter<char> : std::true_type {};

    template<>
    struct TIsCharacter<char16_t> : std::true_type {};

    template<>
    struct TIsCharacter<char32_t> : std::true_type {};

    template<>
    struct TIsCharacter<wchar_t> : std::true_type {};

    template<typename T>
    concept Character = TIsCharacter<T>::value;
}
