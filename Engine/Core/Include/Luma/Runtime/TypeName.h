#pragma once
#include "Luma/Runtime/Types.h"
#include "Luma/Containers/StringView.h"
#include <array>
#include <string_view>

// Compile-time type names and ids for *any* type, no macro required.
//
//   typeNameOf<Luma::TransformComponent>() -> "Luma::TransformComponent"
//   typeIdOf<Luma::TransformComponent>()   -> FNV-1a hash of that name
//
// Names are fully qualified and stripped of MSVC's "class "/"struct "/"enum "/"union " prefixes.
// Plain (non-template) class names are identical across MSVC, Clang and GCC, so their ids are
// safe to serialize. Template and builtin type spellings are compiler-specific
// (e.g. "Foo<int,float>" vs "Foo<int, float>", "unsigned __int64" vs "unsigned long long"):
// don't persist those ids.

namespace Luma
{
    namespace Detail
    {
        constexpr uint64_t hashTypeName(const std::string_view name)
        {
            uint64_t hash = 0xcbf29ce484222325ull;
            for (const char c : name)
            {
                hash ^= static_cast<uint8_t>(c);
                hash *= 0x100000001b3ull;
            }
            return hash;
        }
    }

    struct TypeId
    {
        uint64_t value = 0;

        constexpr TypeId() = default;
        constexpr explicit TypeId(const uint64_t id) : value(id) {}

        static constexpr TypeId fromName(const std::string_view name) { return TypeId(Detail::hashTypeName(name)); }

        constexpr bool isValid() const { return value != 0; }
        constexpr bool operator==(const TypeId&) const = default;
        constexpr explicit operator uint64_t() const { return value; }
    };

    template<typename Key>
    struct Hasher;

    template<>
    struct Hasher<TypeId>
    {
        uint64_t operator()(const TypeId& id) const { return id.value; }
    };

    namespace Detail
    {
        template<typename T>
        constexpr std::string_view rawTypeName()
        {
#if defined(_MSC_VER) && !defined(__clang__)
            return __FUNCSIG__;
#else
            return __PRETTY_FUNCTION__;
#endif
        }

        // Locate where T appears in the signature by probing with a known type.
        inline constexpr std::string_view c_ProbeName = rawTypeName<void>();
        inline constexpr size_t c_TypeNamePrefix = c_ProbeName.find("void");
        inline constexpr size_t c_TypeNameSuffix = c_ProbeName.size() - c_TypeNamePrefix - 4;
        static_assert(c_TypeNamePrefix != std::string_view::npos, "Unsupported compiler for typeNameOf<T>()");

        template<typename T>
        constexpr std::string_view extractTypeName()
        {
            constexpr std::string_view raw = rawTypeName<T>();
            return raw.substr(c_TypeNamePrefix, raw.size() - c_TypeNamePrefix - c_TypeNameSuffix);
        }

        constexpr bool isIdentifierChar(const char c)
        {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
        }

        // Length of the elaborated-type keyword ("class ", ...) starting at index, or 0.
        constexpr size_t keywordAt(const std::string_view name, const size_t index)
        {
            if (index > 0 && isIdentifierChar(name[index - 1]))
                return 0;
            for (const std::string_view keyword : { std::string_view("class "), std::string_view("struct "), std::string_view("enum "), std::string_view("union ") })
            {
                if (name.substr(index, keyword.size()) == keyword)
                    return keyword.size();
            }
            return 0;
        }

        // Writes the cleaned name into out (if non-null) and returns its length.
        constexpr size_t cleanTypeName(const std::string_view name, char* out)
        {
            size_t length = 0;
            for (size_t i = 0; i < name.size();)
            {
                if (const size_t skip = keywordAt(name, i))
                {
                    i += skip;
                    continue;
                }
                if (out) out[length] = name[i];
                ++length;
                ++i;
            }
            return length;
        }

        template<typename T>
        struct TypeNameStorage
        {
            static constexpr std::string_view raw = extractTypeName<T>();
            static constexpr size_t length = cleanTypeName(raw, nullptr);
            static constexpr std::array<char, length + 1> value = []
            {
                std::array<char, length + 1> result{};
                cleanTypeName(raw, result.data());
                return result;
            }();
        };
    }

    // Fully qualified, null-terminated name of T.
    template<typename T>
    constexpr StringView typeNameOf()
    {
        using Storage = Detail::TypeNameStorage<T>;
        return StringView(Storage::value.data(), Storage::length);
    }

    template<typename T>
    constexpr TypeId typeIdOf()
    {
        using Storage = Detail::TypeNameStorage<T>;
        return TypeId::fromName(std::string_view(Storage::value.data(), Storage::length));
    }
}
