#pragma once
#include "Luma/Runtime/Assertion.h"
#include <cstdint>
#include <type_traits>


namespace Luma
{
    template<typename T, uint64_t N>
    class TStaticArray
    {
    public:
        using SizeType = uint64_t;

        template<typename... U> requires (std::is_same_v<U, T> && ...) && (sizeof...(U) <= N)
        constexpr TStaticArray(U&&... elements)
            : m_Data{std::forward<U>(elements)...}
        {

        }

        constexpr SizeType size() const { return m_Count * sizeof(T); }
        SizeType count() const { return m_Count; }
        bool isEmpty() const { return m_Count == 0; }
        bool isFull() const { return m_Count == N; }

        bool add(const T& element)
        {
            if (isFull()) return false;
            m_Data[m_Count++] = element;
            return true;
        }

        bool add(T&& element)
        {
            if (isFull()) return false;
            m_Data[m_Count++] = std::move(element);
            return true;
        }

        bool addRange(const T* elements, SizeType count)
        {
            if (m_Count + count > N) return false;
            for (SizeType i = 0; i < count; ++i)
                m_Data[m_Count++] = elements[i];
            return true;
        }

        T& getAt(uint32_t index)
        {
            LUMA_ASSERT(index < m_Count, "Index out of bounds");
            return m_Data[index];
        }

        const T& getAt(uint32_t index) const
        {
            LUMA_ASSERT(index < m_Count, "Index out of bounds");
            return m_Data[index];
        }

        void removeRange(uint32_t start, uint32_t count)
        {
            LUMA_ASSERT(start < m_Count, "Index out of bounds!");
            LUMA_ASSERT(start + count <= m_Count, "Index out of bounds");
            std::move(m_Data + start + count, m_Data + m_Count, m_Data + start);
            m_Count -= count;
        }

        void removeAt(uint32_t index)
        {
            removeRange(index, 1);
        }

        T& first()
        {
            if (isEmpty()) throw;
            return m_Data[0];
        }

        T& last()
        {
            if (isEmpty()) throw;
            return m_Data[m_Count - 1];
        }

        T* begin() { return &m_Data[0]; }
        const T* begin() const { return &m_Data[0]; }

        T* end() { return &m_Data[m_Count - 1]; }
        const T* end() const { return &m_Data[m_Count - 1]; }
    private:
        T m_Data[N] = {};
        uint64_t m_Count = 0;
    };
}
