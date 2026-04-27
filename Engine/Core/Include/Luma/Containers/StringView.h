#pragma once
#include "Character.h"
#include "String.h"

namespace Luma
{
    template<Character T>
    class TStringView
    {
        using StringType = TString<T>;
        using CharacterType = StringType::CharacterType;
        using PointerType = StringType::PointerType;
        using ConstPointerType = const CharacterType*;
        using SizeType = StringType::SizeType;
    public:
        TStringView() = default;
        TStringView(const StringType& string) : m_Data(string.data()), m_Count(string.count()) { }
        constexpr TStringView(ConstPointerType data) : m_Data(data), m_Count(strlen(data)){}
        constexpr TStringView(ConstPointerType data, SizeType count) : m_Data(data), m_Count(count){}
        constexpr TStringView(decltype(nullptr)) : m_Data(nullptr), m_Count(0){}

        TStringView(const TStringView&) = default;
        TStringView(TStringView&&) noexcept = default;
        TStringView& operator=(const TStringView&) = default;
        TStringView& operator=(TStringView&&) noexcept = default;

        bool operator==(const TStringView& Other) const
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
            LUMA_ASSERT(index < m_Count, "Index out of bounds!");
            return m_Data[index];
        }

        SizeType find(StringType::CharacterType character)
        {
            std::basic_string_view<CharacterType> view(m_Data, m_Count);
            return view.find(character);
        }

        SizeType find(const TStringView& string) const
        {
            std::basic_string_view<CharacterType> view(m_Data, m_Count);
            std::basic_string_view<CharacterType> otherView(string.m_Data, string.m_Count);
            return view.find(otherView);
        }

        SizeType find(SizeType index, const TStringView& str) const
        {
            std::basic_string_view<CharacterType> view(m_Data + index, m_Count);
            std::basic_string_view<CharacterType> otherView(str.m_Data, str.m_Count);
            return view.find(otherView);
        }

        SizeType findLast(const TStringView& string) const
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

        TStringView subview(SizeType index) const
        {
            LUMA_ASSERT(index < m_Count, "Index out of bounds!");
            return TStringView(m_Data + index, m_Count - index);
        }

        TStringView subview(SizeType index, SizeType length) const
        {
            LUMA_ASSERT(index <= m_Count, "Index out of bounds!");
            LUMA_ASSERT(index + length <= m_Count, "Out of bounds!");
            return TStringView(m_Data + index, length);
        }

        bool startsWith(const TStringView& string) const
        {
            return find(string) == 0;
        }

        bool endsWith(const TStringView& string) const
        {
            return findLast(string) != SizeType(-1);
        }

        friend std::basic_ostream<CharacterType>& operator<<(std::basic_ostream<CharacterType>& os, const TStringView& string)
        {
            os.write(string.m_Data, string.m_Count);
            os.flush();
            return os;
        }

        static constexpr const TStringView& empty()
        {
            static constexpr TStringView empty;
            return empty;
        }

    private:
        ConstPointerType m_Data = nullptr;
        SizeType m_Count = 0;
    };

    using FStringView = TStringView<char>;
    using FStringView16 = TStringView<char16_t>;
    using FStringView32 = TStringView<char32_t>;
    using FWideStringView = TStringView<wchar_t>;
}
