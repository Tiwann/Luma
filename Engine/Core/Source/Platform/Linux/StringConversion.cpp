#include "Luma/Containers/StringConversion.h"
#include <codecvt>
#include <locale>

namespace Luma
{
    FWideString stringConvertToWide(const FString& from)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring wide = conv.from_bytes(from.begin(), from.end());
        return FWideString(wide.data(), wide.size());
    }

    FString stringConvertToMultibyte(const FWideString& from)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring wide(from.data(), (from.count()));
        std::string mb = conv.to_bytes(wide);
        return FString(mb.data(), mb.size());
    }

    FWideString stringConvertToWide(const FStringView& from)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring wide = conv.from_bytes(std::string(from.data(), from.count()));
        return FWideString(wide.data(), wide.size());
    }

    FString stringConvertToMultibyte(const FWideStringView& from)
    {
        std::wstring wide(from.data(), (from.count()));
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::string mb = conv.to_bytes(wide);
        return FString(mb.data(), mb.size());
    }

    template<>
    TString<wchar_t> stringConvert(const TString<char>& from)
    {
        return stringConvertToWide(from);
    }

    template<>
    TString<char> stringConvert(const TString<wchar_t>& from)
    {
        return stringConvertToMultibyte(from);
    }

    template<>
    TString<wchar_t> stringConvert(const TStringView<char>& from)
    {
        return stringConvertToWide(from);
    }

    template<>
    TString<char> stringConvert(const TStringView<wchar_t>& from)
    {
        return stringConvertToMultibyte(from);
    }
}
