#pragma once
#include "Character.h"
#include "String.h"

namespace Luma
{
    template<Character T>
    class TStringViewBase
    {
        using StringType = TStringBase<T>;
        using CharacterType = StringType::CharacterType;
        using PointerType = StringType::PointerType;
        using ConstPointerType = const CharacterType*;
        using StringLiteralType = StringType::StringLiteralType;
        using SizeType = StringType::SizeType;
    public:
        TStringViewBase() = default;
        TStringViewBase(const StringType& string) : m_Data(string.data()), m_Count(string.count()) { }
        constexpr TStringViewBase(ConstPointerType data) : m_Data(data), m_Count(strlen(data)){}
        constexpr TStringViewBase(ConstPointerType data, SizeType count) : m_Data(data), m_Count(count){}
        constexpr TStringViewBase(decltype(nullptr)) : m_Data(nullptr), m_Count(0){}

        TStringViewBase(const TStringViewBase&) = default;
        TStringViewBase(TStringViewBase&&) noexcept = default;
        TStringViewBase& operator=(const TStringViewBase&) = default;
        TStringViewBase& operator=(TStringViewBase&&) noexcept = default;

        bool operator==(const TStringViewBase& Other) const
        {
            return m_Count == Other.m_Count && ::memcpy(m_Data, Other.m_Data, m_Count * StringType::CharacterSize);
        }

        bool isValid() const { return m_Data && m_Count != 0; }
        ConstPointerType data() const { return m_Data; }
        SizeType count() const { return m_Count; }
        SizeType size() const { return m_Count * sizeof(CharacterType); }

        ConstPointerType begin() const { return m_Data; }
        ConstPointerType end() const { return m_Data + m_Count; }

        bool isEmpty() const { return m_Data == nullptr || m_Count == 0; }

        operator const CharacterType*() const { return m_Data; }
        const CharacterType* operator*() const { return m_Data; }

        const CharacterType& operator[](SizeType index) const
        {
            assert(index < m_Count, "Index out of bounds!");
            return m_Data[index];
        }

        SizeType find(StringType::CharacterType character)
        {
            std::basic_string_view<CharacterType> view(m_Data, m_Count);
            return view.find(character);
        }

        SizeType find(const TStringViewBase& string) const
        {
            std::basic_string_view<CharacterType> view(m_Data, m_Count);
            std::basic_string_view<CharacterType> otherView(string.m_Data, string.m_Count);
            return view.find(otherView);
        }

        SizeType find(SizeType index, const TStringViewBase& str) const
        {
            std::basic_string_view<CharacterType> view(m_Data + index, m_Count);
            std::basic_string_view<CharacterType> otherView(str.m_Data, str.m_Count);
            return view.find(otherView);
        }

        SizeType findLast(const TStringViewBase& string) const
        {
            std::basic_string_view<CharacterType> view(m_Data, m_Count);
            std::basic_string_view<CharacterType> otherView(string.m_Data, string.m_Count);
            return view.find_last_of(otherView);
        }

        SizeType findLast(StringType::CharacterType character) const
        {
            std::basic_string_view<CharacterType> view(m_Data, m_Count);
            return view.find_last_of(character);
        }

        TStringViewBase subview(SizeType index) const
        {
            assert(index < m_Count, "Index out of bounds!");
            return TStringViewBase(m_Data + index, m_Count - index);
        }

        TStringViewBase subview(SizeType index, SizeType length) const
        {
            assert(index <= m_Count, "Index out of bounds!");
            assert(index + length <= m_Count, "Out of bounds!");
            return TStringViewBase(m_Data + index, length);
        }

        bool startsWith(const TStringViewBase& string) const
        {
            return find(string) == 0;
        }

        bool endsWith(const TStringViewBase& string) const
        {
            return findLast(string) != SizeType(-1);
        }

        friend std::basic_ostream<CharacterType>& operator<<(std::basic_ostream<CharacterType>& os, const TStringViewBase& string)
        {
            os.write(string.m_Data, string.m_Count);
            os.flush();
            return os;
        }

        static constexpr const TStringViewBase& empty()
        {
            static constexpr TStringViewBase empty;
            return empty;
        }

    private:
        ConstPointerType m_Data = nullptr;
        SizeType m_Count = 0;
    };

    using FStringView = TStringViewBase<char>;
    using FStringView16 = TStringViewBase<char16_t>;
    using FStringView32 = TStringViewBase<char32_t>;
    using FWideStringView = TStringViewBase<wchar_t>;
}
