#include "Luma/Containers/StringFormat.h"


std::format_context::iterator std::formatter<Luma::FString>::format(const Luma::FString& str, format_context& ctx) const
{
    return formatter<string_view>::format(string_view(str.data(), str.count()), ctx);
}

std::format_context::iterator std::formatter<Luma::FStringView>::format(const Luma::FStringView& str,format_context& ctx) const
{
    return formatter<string_view>::format(string_view(str.data(), str.count()), ctx);
}
