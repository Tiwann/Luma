#include "Luma/Containers/StringConversion.h"
#include <Windows.h>

namespace Luma
{
    WideString stringConvertToWide(const String& from)
    {
        const uint64_t length = MultiByteToWideChar(CP_UTF8, 0, from.data(), from.count(), nullptr, 0);
        WideString::CharacterType* buffer = new WideString::CharacterType[length] { 0 };
        MultiByteToWideChar(CP_UTF8, 0, from.data(), from.count(), buffer, length);
        WideString result { buffer, length };
        delete[] buffer;
        return result;
    }

    String stringConvertToMultibyte(const WideString& from)
    {
        const uint64_t length = WideCharToMultiByte(CP_UTF8, 0, from.data(), from.count(), nullptr, 0, nullptr, nullptr);
        String::CharacterType* buffer = new String::CharacterType[length] { 0 };
        WideCharToMultiByte(CP_UTF8, 0, from.data(), from.count(), buffer, length, nullptr, nullptr);
        String result { buffer, length };
        delete[] buffer;
        return result;
    }

    WideString stringConvertToWide(const StringView& from)
    {
        const uint64_t length = MultiByteToWideChar(CP_UTF8, 0, from.data(), from.count(), nullptr, 0);
        WideString::CharacterType* buffer = new WideString::CharacterType[length] { };
        MultiByteToWideChar(CP_UTF8, 0, from.data(), from.count(), buffer, length);
        WideString result { buffer, length };
        delete[] buffer;
        return result;
    }

    String stringConvertToMultibyte(const WideStringView& from)
    {
        const uint64_t length = WideCharToMultiByte(CP_UTF8, 0, from.data(), from.count(), nullptr, 0, nullptr, nullptr);
        String::CharacterType* buffer = new String::CharacterType[length] { };
        WideCharToMultiByte(CP_UTF8, 0, from.data(), from.count(), buffer, length, nullptr, nullptr);
        String result { buffer, length };
        delete[] buffer;
        return result;
    }

    template<>
    StringBase<wchar_t> stringConvert(const StringBase<char>& from)
    {
        return stringConvertToWide(from);
    }

    template<>
    StringBase<char> stringConvert(const StringBase<wchar_t>& from)
    {
        return stringConvertToMultibyte(from);
    }

    template<>
    StringBase<wchar_t> stringConvert(const StringViewBase<char>& from)
    {
        return stringConvertToWide(from);
    }

    template<>
    StringBase<char> stringConvert(const StringViewBase<wchar_t>& from)
    {
        return stringConvertToMultibyte(from);
    }
}
