#pragma once
#include "Character.h"
#include "Luma/Runtime/Assertion.h"
#include "Array.h"
#include <string_view>
#include <iostream>

namespace Luma
{
    template<Character T>
    static constexpr uint64_t strlen(const T* data)
    {
        if (!data) return 0;

        const T* ptr = data;
        uint64_t count = 0;
        while (*ptr != 0)
        {
            count++;
            ++ptr;
        }
        return count;
    }
    
    template<Character T>
    class TString
    {
    public:
        using CharacterType = T;
        using SizeType = uint64_t;
        using PointerType = CharacterType*;

        static constexpr SizeType CharacterSize = sizeof(CharacterType);

        TString()
        {
            m_Data = new CharacterType[1]{};
            m_Count = 0;
        }

        TString(const CharacterType* data)
        {
            LUMA_ASSERT(data, "Cannot construct string with nullptr!");
            m_Count = strlen(data);
            m_Data = new CharacterType[m_Count + 1]{};
            memcpy(m_Data, data, m_Count * CharacterSize);
        }

        explicit TString(const SizeType count) : m_Count(count)
        {
            m_Data = new CharacterType[m_Count + 1]{};
        }

        TString(CharacterType* data, SizeType count)
        {
            LUMA_ASSERT(data, "Cannot construct string with nullptr!");
            m_Count = count;
            m_Data = new CharacterType[m_Count + 1]{};
            memcpy(m_Data, data, m_Count * CharacterSize);
        }

        TString(const TString& other)
        {
            delete[] m_Data;
            m_Data = new CharacterType[other.m_Count + 1]{};
            memcpy(m_Data, other.m_Data, other.m_Count * CharacterSize);
            m_Count = other.m_Count;
        }

        TString(TString&& Other) noexcept
        {
            m_Data = Other.m_Data;
            m_Count = Other.m_Count;
            Other.m_Data = nullptr;
            Other.m_Count = 0;
        }

        TString& operator=(const TString& other)
        {
            if(this == &other)
                return *this;

            if(m_Data) delete[] m_Data;
            m_Data = new CharacterType[other.m_Count + 1]{};
            memcpy(m_Data, other.m_Data, other.m_Count * CharacterSize);
            m_Count = other.m_Count;
            return *this;
        }

        TString& operator=(TString&& other) noexcept
        {
            if(this == &other)
                return *this;

            delete[] m_Data;

            m_Data = other.m_Data;
            m_Count = other.m_Count;
            other.m_Data = nullptr;
            other.m_Count = 0;
            return *this;
        }

        template<SizeType N>
        TString& operator=(CharacterType (&&buffer)[N])
        {
            const SizeType count = strlen(buffer);
            if (count < N)
            {
                delete[] m_Data;
                m_Data = new CharacterType[count];
            }

            ::memmove(m_Data, buffer, count * CharacterSize);
            m_Data[m_Count] = 0;
            m_Count = count;
            return *this;
        }

        ~TString()
        {
            delete[] m_Data;
            m_Count = 0;
        }

        CharacterType& operator[](SizeType index)
        {
            LUMA_ASSERT(index <= m_Count, "Index out of bounds");
            return m_Data[index];
        }

        const CharacterType& operator[](SizeType index) const
        {
            LUMA_ASSERT(index <= m_Count, "Index out of bounds");
            return m_Data[index];
        }

        bool operator==(const TString& other) const
        {
            return std::strcmp(m_Data, other.m_Data) == 0 && m_Count == other.m_Count;
        }

        bool isEmpty() const { return m_Count == 0; }

        CharacterType* data() { return m_Data; }
        const CharacterType* data() const { return m_Data; }

        CharacterType* operator*() { return m_Data; }
        const CharacterType* operator*() const { return m_Data; }

        SizeType count() const { return m_Count; }
        SizeType size() const { return m_Count * CharacterSize; }


        TString& resize(const SizeType newCount)
        {
            if (m_Count == newCount) return *this;

            if (m_Count > newCount)
            {
                CharacterType* newData = new CharacterType [newCount + 1]{0};
                ::memcpy(newData, m_Data, m_Count * CharacterSize);
                delete [] m_Data;
                m_Data = newData;
                m_Count = newCount;
                return *this;
            }

            if (m_Count < newCount)
            {
                CharacterType* newData = new CharacterType [newCount + 1]{0};
                ::memcpy(newData, m_Data, newCount * CharacterSize);
                delete [] m_Data;
                m_Data = newData;
                m_Count = newCount;
                return *this;
            }

            return *this;
        }

        TString& append(const CharacterType* data)
        {
            LUMA_ASSERT(data, "Cannot append string with nullptr string literal!");
            const SizeType dataCount = strlen(data);
            const SizeType newCount = m_Count + dataCount;
            CharacterType* newData = new CharacterType[newCount + 1]{};
            memcpy(newData, m_Data, m_Count * CharacterSize);
            memcpy(newData + m_Count, data, dataCount * CharacterSize);
            delete [] m_Data;
            m_Data = newData;
            m_Count = newCount;
            return *this;
        }


        TString& append(CharacterType character)
        {
            const SizeType newCount = m_Count + 1;
            CharacterType* newData = new CharacterType[newCount + 1]{};
            memcpy(newData, m_Data, m_Count * CharacterSize);
            memcpy(newData + m_Count, &character, CharacterSize);
            delete [] m_Data;
            m_Data = newData;
            m_Count = newCount;
            return *this;
        }

        TString& append(const TString& string)
        {
            const SizeType dataCount = string.count();
            const SizeType newCount = m_Count + dataCount;
            CharacterType* newData = new CharacterType[newCount + 1]{};
            memcpy(newData, m_Data, m_Count * CharacterSize);
            memcpy(newData + m_Count, string.data(), dataCount * CharacterSize);
            delete [] m_Data;
            m_Data = newData;
            m_Count = newCount;
            return *this;
        }

        TString substr(SizeType begin, SizeType end) const
        {
            LUMA_ASSERT(begin < m_Count && begin + (end - begin) <= m_Count, "Indices out of bounds!");
            const SizeType newCount = end - begin + 1;
            CharacterType* newData = new CharacterType[newCount + 1]{};
            memcpy(newData, m_Data + begin, newCount * CharacterSize);
            return {newData, newCount};
        }

        TString substr(const SizeType begin) const
        {
            return substr(begin, m_Count);
        }

        SizeType find(CharacterType character) const
        {
            for(SizeType i = 0; i < m_Count; ++i)
            {
                if(m_Data[i] == character)
                    return i;
            }
            return -1;
        }

        SizeType find(const TString& string) const
        {
            std::basic_string_view<CharacterType> view(m_Data, m_Count);
            std::basic_string_view<CharacterType> otherView(string.m_Data, string.m_Count);
            return view.find(otherView);
        }

        SizeType find(SizeType index, const TString& string) const
        {
            std::basic_string_view<CharacterType> view(m_Data + index, m_Count);
            std::basic_string_view<CharacterType> otherView(string.m_Data, string.m_Count);
            return view.find(otherView);
        }

        SizeType findLast(const TString& string) const
        {
            std::basic_string_view<CharacterType> view(m_Data, m_Count);
            std::basic_string_view<CharacterType> otherView(string.m_Data, string.m_Count);
            return view.find_last_of(otherView);
        }

        bool endsWith(const TString& string) const
        {
            return findLast(string) != SizeType(-1);
        }

        bool startsWith(const TString& string) const
        {
            return find(string) == 0;
        }

        SizeType occurrencesOf(CharacterType character) const
        {
            SizeType result = 0;
            for(SizeType i = 0; i < m_Count; ++i)
            {
                CharacterType current = m_Data[i];
                if(current == character)
                    result++;
            }
            return result;
        }

        TString& replace(const TString& from, const TString& to)
        {
            const SizeType index = find(from);
            if(index == -1ULL) return *this;

            if (to.count() <= from.count())
            {
                memcpy(m_Data + index, *to, to.size());
                return *this;
            }

            const SizeType delta = to.count() - from.count();
            const SizeType newCount = m_Count + delta;
            CharacterType* newData = new CharacterType[newCount]{};

            CharacterType* dest = newData;
            CharacterType* src = m_Data;
            SizeType size = index * CharacterSize;
            memcpy(dest, src, size);

            dest = newData + index * CharacterSize;
            src = const_cast<CharacterType*>(*to);
            size = to.size();
            memcpy(dest, src, size);

            dest = newData + index * CharacterSize + to.size();
            src = m_Data + index * CharacterSize + from.size();
            size = m_Count * CharacterSize - (index * CharacterSize + from.size());
            memcpy(dest, src, size);

            delete [] m_Data;
            m_Data = newData;
            m_Count = newCount;
            return *this;
        }

        TString& replace(SizeType index, SizeType count, const TString& to)
        {
            LUMA_ASSERT(index < m_Count && index + count <= m_Count, "Range is out of bounds!");
            if (index == -1ULL) return *this;

            if (to.count() <= count)
            {
                memcpy(m_Data + index, *to, to.size());
                return *this;
            }

            const SizeType delta = to.count() - count;
            const SizeType newCount = m_Count + delta;
            CharacterType* newData = new CharacterType[newCount]{};

            CharacterType* dest = newData;
            CharacterType* src = m_Data;
            SizeType size = index * CharacterSize;
            memcpy(dest, src, size);

            dest = newData + index * CharacterSize;
            src = const_cast<CharacterType*>(*to);
            size = to.size();
            memcpy(dest, src, size);

            dest = newData + index * CharacterSize + to.size();
            src = m_Data + index * CharacterSize + count * CharacterSize;
            size = m_Count * CharacterSize - (index * CharacterSize + count * CharacterSize);
            memcpy(dest, src, size);

            delete [] m_Data;
            m_Data = newData;
            m_Count = newCount;
            return *this;
        }

        TString& replaceAll(const TString& from, const TString& to)
        {
            SizeType index = 0;
            while ((index = find(index, from)) != -1ULL)
            {
                replace(index, m_Count - index, to);
            }
            return *this;
        }

        TString& replaceAll(CharacterType from, CharacterType to)
        {
            if (from == to) return *this;
            if (!m_Data || m_Count <= 0) return *this;

            CharacterType* ptr = m_Data;
            while (ptr < m_Data + m_Count)
            {
                if (*ptr == from)
                    *ptr = to;
                ++ptr;
            }
            return *this;
        }

        TString& remove(SizeType from, SizeType to)
        {
            LUMA_ASSERT(from < to, "Range is illegal");
            LUMA_ASSERT(from < m_Count, "Range is illegal");
            LUMA_ASSERT(to <= m_Count, "Range is illegal");
            LUMA_ASSERT(from + (to - from) <= m_Count, "Range is illegal");
            const SizeType delta = to - from;

            const CharacterType* src = m_Data + to;
            CharacterType* dest = m_Data + from;
            const SizeType size = (m_Count - to) * CharacterSize;
            ::memmove(dest, src, size);
            m_Count -= delta;
            m_Data[m_Count] = 0;
            return *this;
        }

        TString& remove(const TString& string)
        {
            const SizeType index = find(string);
            if (index == -1ULL) return *this;
            Remove(index, index + string.count());
            return *this;
        }

        T* begin() { return m_Data; }
        T* end() { return m_Data + m_Count; }
        const T* begin() const { return m_Data; }
        const T* end() const { return m_Data + m_Count; }
        TString& operator+(const TString& other)
        {
            return append(other);
        }

        TString& trimEnd(CharacterType character)
        {
            SizeType count = 0;
            for (uint64_t index = m_Count - 1; index > 0; --index)
            {
                if (m_Data[index] != character)
                    break;
                count++;
            }

            if (count == 0)
                return *this;

            m_Count -= count;
            m_Data[m_Count] = 0;
            return *this;
        }

        TString trimStart(CharacterType character) const
        {
            TString copy(*this);
            SizeType count = 0;
            while (count < copy.m_Count && copy.m_Data[count] == character)
                ++count;

            if (count == 0)
                return copy;

            memmove(copy.m_Data, copy.m_Data + count, (copy.m_Count - count) * CharacterSize);
            copy.m_Count -= count;
            copy.m_Data[m_Count] = 0;
            return copy;
        }

        TString trimStart(const TArray<CharacterType>& characters) const
        {
            TString copy(*this);
            SizeType count = 0;
            while (count < copy.m_Count && characters.Contains(copy.m_Data[count]))
                ++count;

            if (count == 0)
                return *this;

            ::memmove(copy.m_Data, copy.m_Data + count, (copy.m_Count - count) * CharacterSize);
            copy.m_Count -= count;
            copy.m_Data[copy.m_Count] = 0;
            return copy;
        }

        friend std::basic_ostream<CharacterType>& operator<<(std::basic_ostream<CharacterType>& os, const TString& string)
        {
            os.write(string.m_Data, string.m_Count * CharacterSize);
            os.flush();
            return os;
        }

    private:
        CharacterType* m_Data = nullptr;
        uint64_t m_Count = 0;
    };

    using FString = TString<char>;
    using FString8 = TString<char>;
    using FString16 = TString<char16_t>;
    using FString32 = TString<char32_t>;
    using FWideString = TString<wchar_t>;
}






