#pragma once
#include "String.h"
#include "StringView.h"
#include <format>

template<>
struct std::formatter<Luma::FString> : std::formatter<std::string_view>
{
    std::format_context::iterator format(const Luma::FString& str, format_context& ctx) const;
};

template<>
struct std::formatter<Luma::FStringView> : std::formatter<std::string_view>
{
    std::format_context::iterator format(const Luma::FStringView& str, format_context& ctx) const;
};

namespace Luma
{
    template <typename... Args>
    FString strfmt(const FStringView& format, const Args&... args)
    {
        std::string formatted = std::vformat(static_cast<std::string_view>(format), std::make_format_args(args...));
        return {formatted.data(), formatted.size()};
    }

    template<Character T, typename... Args>
    uint32_t strscanf(const TStringView<T> string, const TStringView<T> format, Args*... args)
    {
        if constexpr (typeid(T) == typeid(char))
        {
            return std::sscanf(string.Data(), format.Data(), args...);
        } else if constexpr (typeid(T) == typeid(wchar_t))
        {
            return std::swscanf(string.Data(), format.Data(), args...);
        } else
        {
            static_assert(false, "Cannot sscanf with this string type!");
            return 0;
        }
    }
}
