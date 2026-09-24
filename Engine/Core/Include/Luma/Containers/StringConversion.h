#pragma once
#include "String.h"
#include "StringView.h"

namespace Luma
{
    WideString stringConvertToWide(const String& from);
    String stringConvertToMultibyte(const WideString& from);
    WideString stringConvertToWide(const StringView& from);
    String stringConvertToMultibyte(const WideStringView& from);

    template<Character T>
    StringBase<T> stringFromView(StringViewBase<T> view)
    {
        return StringBase<T>(const_cast<T*>(view.data()), view.count());
    }

    template<Character To, Character From> requires (!std::is_same_v<To, From>)
    StringBase<To> stringConvert(const StringBase<From>& from);

    template<Character To, Character From> requires (!std::is_same_v<To, From>)
    StringBase<To> stringConvert(const StringViewBase<From>& from);
}
