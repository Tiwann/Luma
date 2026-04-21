#pragma once
#include <cstdint>
#include <type_traits>

namespace Luma
{
    template <typename T>
    class TBufferView
    {
    public:
        constexpr TBufferView() = default;

        TBufferView(const T* data, uint64_t count) : m_Data(data), m_Count(count){}
        TBufferView(const TBufferView& buffer) : m_Data(buffer.m_Data), m_Count(buffer.m_Count){}

        const T* begin() const { return m_Data; }
        const T* end() const { return m_Data + m_Count; }

        template <typename U> requires std::is_convertible_v<T, U>
        TBufferView<U> as() const
        {
            return TBufferView<U>(m_Data, m_Count * (sizeof(T) / sizeof(U)));
        }

        uint64_t count() const { return m_Count; }
        uint64_t size() const { return m_Count * sizeof(T); }
        const T* data() const { return m_Data; }
        const T& operator[](uint64_t Index) const { return m_Data[Index]; }

        bool isEmpty() const { return !m_Data || m_Count == 0; }
    private:
        const T* m_Data = nullptr;
        uint64_t m_Count = 0;
    };
}
