#include "Luma/Containers/StringConversion.h"
#include <Windows.h>

namespace Luma
{
    FWideString stringConvertToWide(const FString& from)
    {
        const uint64_t length = MultiByteToWideChar(CP_UTF8, 0, from.data(), from.count(), nullptr, 0);
        FWideString::CharacterType* buffer = new FWideString::CharacterType[length] { 0 };
        MultiByteToWideChar(CP_UTF8, 0, from.data(), from.count(), buffer, length);
        FWideString result { buffer, length };
        delete[] buffer;
        return result;
    }

    FString stringConvertToMultibyte(const FWideString& from)
    {
        const uint64_t length = WideCharToMultiByte(CP_UTF8, 0, from.data(), from.count(), nullptr, 0, nullptr, nullptr);
        FString::CharacterType* buffer = new FString::CharacterType[length] { 0 };
        WideCharToMultiByte(CP_UTF8, 0, from.data(), from.count(), buffer, length, nullptr, nullptr);
        FString result { buffer, length };
        delete[] buffer;
        return result;
    }

    FWideString stringConvertToWide(const FStringView& from)
    {
        const uint64_t length = MultiByteToWideChar(CP_UTF8, 0, from.data(), from.count(), nullptr, 0);
        FWideString::CharacterType* buffer = new FWideString::CharacterType[length] { };
        MultiByteToWideChar(CP_UTF8, 0, from.data(), from.count(), buffer, length);
        FWideString result { buffer, length };
        delete[] buffer;
        return result;
    }

    FString stringConvertToMultibyte(const FWideStringView& from)
    {
        const uint64_t length = WideCharToMultiByte(CP_UTF8, 0, from.data(), from.count(), nullptr, 0, nullptr, nullptr);
        FString::CharacterType* buffer = new FString::CharacterType[length] { };
        WideCharToMultiByte(CP_UTF8, 0, from.data(), from.count(), buffer, length, nullptr, nullptr);
        FString result { buffer, length };
        delete[] buffer;
        return result;
    }

    template<>
    TStringBase<wchar_t> stringConvert(const TStringBase<char>& from)
    {
        return stringConvertToWide(from);
    }

    template<>
    TStringBase<char> stringConvert(const TStringBase<wchar_t>& from)
    {
        return stringConvertToMultibyte(from);
    }

    template<>
    TStringBase<wchar_t> stringConvert(const TStringViewBase<char>& from)
    {
        return stringConvertToWide(from);
    }

    template<>
    TStringBase<char> stringConvert(const TStringViewBase<wchar_t>& from)
    {
        return stringConvertToMultibyte(from);
    }
}
