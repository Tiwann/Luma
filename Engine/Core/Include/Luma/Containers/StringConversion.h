#pragma once
#include "String.h"
#include "StringView.h"

namespace Luma
{
    FWideString stringConvertToWide(const FString& from);
    FString stringConvertToMultibyte(const FWideString& from);
    FWideString stringConvertToWide(const FStringView& from);
    FString stringConvertToMultibyte(const FWideStringView& from);

    template<Character T>
    TStringBase<T> stringFromView(TStringViewBase<T> view)
    {
        return TStringBase<T>(const_cast<T*>(view.Data()), view.Count());
    }

    template<Character To, Character From> requires (!std::is_same_v<To, From>)
    TStringBase<To> stringConvert(const TStringBase<From>& from);

    template<Character To, Character From> requires (!std::is_same_v<To, From>)
    TStringBase<To> stringConvert(const TStringViewBase<From>& from);
}
