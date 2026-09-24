#pragma once
#include <cstdint>
#include <type_traits>

namespace Luma
{
    template <typename T>
    class BufferView
    {
    public:
        constexpr BufferView() = default;

        constexpr BufferView(const T* data, uint64_t count) : m_Data(data), m_Count(count){}
        constexpr BufferView(const T& element) : m_Data(&element), m_Count(1){}
        BufferView(const BufferView& buffer) : m_Data(buffer.m_Data), m_Count(buffer.m_Count){}


        const T* begin() const { return m_Data; }
        const T* end() const { return m_Data + m_Count; }

        template <typename U> requires std::is_convertible_v<T, U>
        BufferView<U> as() const
        {
            return BufferView<U>(m_Data, m_Count * (sizeof(T) / sizeof(U)));
        }

        const T& first() const { return m_Data[0]; }
        const T& last() const { return m_Data[m_Count]; }

        uint64_t count() const { return m_Count; }
        uint64_t size() const { return m_Count * sizeof(T); }
        const T* data() const { return m_Data; }
        const T& operator[](uint64_t Index) const { return m_Data[Index]; }

        bool isEmpty() const { return !m_Data || m_Count == 0; }
    protected:
        const T* m_Data = nullptr;
        uint64_t m_Count = 0;
    };
}
