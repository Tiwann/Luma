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
    TString<T> stringFromView(TStringView<T> view)
    {
        return TString<T>(const_cast<T*>(view.data()), view.count());
    }

    template<Character To, Character From> requires (!std::is_same_v<To, From>)
    TString<To> stringConvert(const TString<From>& from);

    template<Character To, Character From> requires (!std::is_same_v<To, From>)
    TString<To> stringConvert(const TStringView<From>& from);
}
