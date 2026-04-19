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
    class TStringBase
    {
    public:
        using CharacterType = T;
        using SizeType = uint64_t;
        using StringLiteralType = const CharacterType*;
        using PointerType = CharacterType*;
        
        static constexpr SizeType CharacterSize = sizeof(CharacterType);
        
        TStringBase()
        {
            m_Data = new CharacterType[1]{};
            m_Count = 0;
        }
        
        TStringBase(StringLiteralType data)
        {
            assert(data, "Cannot construct string with nullptr!");
            m_Count = strlen(data);
            m_Data = new CharacterType[m_Count + 1]{};
            memcpy(m_Data, data, m_Count * CharacterSize);
        }

        explicit TStringBase(const SizeType count) : m_Count(count)
        {
            m_Data = new CharacterType[m_Count + 1]{};
        }

        TStringBase(CharacterType* data, SizeType count)
        {
            assert(data, "Cannot construct string with nullptr!");
            m_Count = count;
            m_Data = new CharacterType[m_Count + 1]{};
            memcpy(m_Data, data, m_Count * CharacterSize);
        }

        TStringBase(const TStringBase& other)
        {
            delete[] m_Data;
            m_Data = new CharacterType[other.m_Count + 1]{};
            memcpy(m_Data, other.m_Data, other.m_Count * CharacterSize);
            m_Count = other.m_Count;
        }

        TStringBase(TStringBase&& Other) noexcept
        {
            m_Data = Other.m_Data;
            m_Count = Other.m_Count;
            Other.m_Data = nullptr;
            Other.m_Count = 0;
        }

        TStringBase& operator=(const TStringBase& other)
        {
            if(this == &other)
                return *this;

            if(m_Data) delete[] m_Data;
            m_Data = new CharacterType[other.m_Count + 1]{};
            memcpy(m_Data, other.m_Data, other.m_Count * CharacterSize);
            m_Count = other.m_Count;
            return *this;
        }

        TStringBase& operator=(TStringBase&& other) noexcept
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
        TStringBase& operator=(CharacterType (&&buffer)[N])
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

        ~TStringBase()
        {
            delete[] m_Data;
            m_Count = 0;
        }

        CharacterType& operator[](SizeType index)
        {
            assert(index <= m_Count, "Index out of bounds");
            return m_Data[index];
        }
        
        const CharacterType& operator[](SizeType index) const
        {
            assert(index <= m_Count, "Index out of bounds");
            return m_Data[index];
        }

        bool operator==(const TStringBase& other) const
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


        TStringBase& resize(const SizeType newCount)
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
        
        TStringBase& append(StringLiteralType data)
        {
            assert(data, "Cannot append string with nullptr string literal!");
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

        
        TStringBase& append(CharacterType character)
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

        TStringBase& append(const TStringBase& string)
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
        
        TStringBase substr(SizeType begin, SizeType end) const
        {
            assert(begin < m_Count && begin + (end - begin) <= m_Count, "Indices out of bounds!");
            const SizeType newCount = end - begin + 1;
            CharacterType* newData = new CharacterType[newCount + 1]{};
            memcpy(newData, m_Data + begin, newCount * CharacterSize);
            return {newData, newCount};
        }

        TStringBase substr(const SizeType begin) const
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

        SizeType find(const TStringBase& string) const
        {
            std::basic_string_view<CharacterType> view(m_Data, m_Count);
            std::basic_string_view<CharacterType> otherView(string.m_Data, string.m_Count);
            return view.find(otherView);
        }

        SizeType find(SizeType index, const TStringBase& string) const
        {
            std::basic_string_view<CharacterType> view(m_Data + index, m_Count);
            std::basic_string_view<CharacterType> otherView(string.m_Data, string.m_Count);
            return view.find(otherView);
        }

        SizeType findLast(const TStringBase& string) const
        {
            std::basic_string_view<CharacterType> view(m_Data, m_Count);
            std::basic_string_view<CharacterType> otherView(string.m_Data, string.m_Count);
            return view.find_last_of(otherView);
        }

        bool endsWith(const TStringBase& string) const
        {
            return findLast(string) != SizeType(-1);
        }

        bool startsWith(const TStringBase& string) const
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

        TStringBase& replace(const TStringBase& from, const TStringBase& to)
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

        TStringBase& replace(SizeType index, SizeType count, const TStringBase& to)
        {
            assert(index < m_Count && index + count <= m_Count, "Range is out of bounds!");
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

        TStringBase& replaceAll(const TStringBase& from, const TStringBase& to)
        {
            SizeType index = 0;
            while ((index = find(index, from)) != -1ULL)
            {
                replace(index, m_Count - index, to);
            }
            return *this;
        }

        TStringBase& replaceAll(CharacterType from, CharacterType to)
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

        TStringBase& remove(SizeType from, SizeType to)
        {
            assert(from < to, "Range is illegal");
            assert(from < m_Count, "Range is illegal");
            assert(to <= m_Count, "Range is illegal");
            assert(from + (to - from) <= m_Count, "Range is illegal");
            const SizeType delta = to - from;
            
            const CharacterType* src = m_Data + to;
            CharacterType* dest = m_Data + from;
            const SizeType size = (m_Count - to) * CharacterSize;
            ::memmove(dest, src, size);
            m_Count -= delta;
            m_Data[m_Count] = 0;
            return *this;
        }

        TStringBase& remove(const TStringBase& string)
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
        TStringBase& operator+(const TStringBase& other)
        {
            return append(other);
        }

        TStringBase& trimEnd(CharacterType character)
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

        TStringBase trimStart(CharacterType character) const
        {
            TStringBase copy(*this);
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

        TStringBase trimStart(const TArray<CharacterType>& characters) const
        {
            TStringBase copy(*this);
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

        friend std::basic_ostream<CharacterType>& operator<<(std::basic_ostream<CharacterType>& os, const TStringBase& string)
        {
            os.write(string.m_Data, string.m_Count * CharacterSize);
            os.flush();
            return os;
        }

    private:
        CharacterType* m_Data = nullptr;
        uint64_t m_Count = 0;
    };

    using FString = TStringBase<char>;
    using FString8 = TStringBase<char>;
    using FString16 = TStringBase<char16_t>;
    using FString32 = TStringBase<char32_t>;
    using FWideString = TStringBase<wchar_t>;
}






