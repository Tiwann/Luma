#include "Luma/Containers/StringFormat.h"


std::format_context::iterator std::formatter<Luma::String>::format(const Luma::String& str, format_context& ctx) const
{
    return formatter<string_view>::format(string_view(str.data(), str.count()), ctx);
}

std::format_context::iterator std::formatter<Luma::StringView>::format(const Luma::StringView& str,format_context& ctx) const
{
    return formatter<string_view>::format(string_view(str.data(), str.count()), ctx);
}
