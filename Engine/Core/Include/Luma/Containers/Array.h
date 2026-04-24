#pragma once
#include "Luma/Core/Export.h"
#include "Luma/Memory/Allocator.h"
#include "Luma/Runtime/Assertion.h"
#include "Luma/Math/Concepts.h"
#include <initializer_list>
#include <algorithm>
#include <functional>


namespace Luma
{
    template<typename T>
    class TArray final
    {
    public:
        using ValueType = T;
        using PointerType = T*;
        using ConstPointerType = const T*;
        using ReferenceType = T&;
        using ConstReferenceType = const T&;
        using SizeType = uint64_t;
        using ForwardType = T&&;
        
        TArray()
        {
            m_Allocated = 1;
            m_Data = new T[m_Allocated]{};
            m_Count = 0;
        }

        explicit TArray(const SizeType count)
        {
            static constexpr auto nearestPow2 = [](SizeType n) -> SizeType
            {
                if (n == 0) return 1;

                --n;
                n |= n >> 1;
                n |= n >> 2;
                n |= n >> 4;
                n |= n >> 8;
                n |= n >> 16;
                return n + 1;
            };
            m_Allocated = nearestPow2(count);
            m_Data = new T[m_Allocated]{};
            m_Count = count;
        }


        TArray(const std::initializer_list<T>& list) : m_Count(list.size()), m_Allocated(list.size())
        {
            m_Data = new T[m_Allocated]{};
            std::copy(list.begin(), list.end(), m_Data);
        }

        TArray(ConstPointerType data, SizeType count) : m_Count(count), m_Allocated(count)
        {
            m_Data = new T[m_Allocated]{};
            std::copy(data, data + count, m_Data);
        }

        TArray(const TArray& other) : m_Count(other.m_Count), m_Allocated(other.m_Allocated)
        {
            m_Data = new T[m_Allocated]{};
            std::copy(other.begin(), other.end(), m_Data);
        }

        TArray(TArray&& other) noexcept
        {
            m_Data = other.m_Data;
            m_Count = other.m_Count;
            m_Allocated = other.m_Allocated;

            other.m_Data = nullptr;
            other.m_Count = 0;
            other.m_Allocated = 0;
        }

        ~TArray()
        {
            delete[] m_Data;
        }

        TArray& operator=(const TArray& other)
        {
            if(this == &other)
                return *this;

            m_Allocated = other.m_Allocated;
            m_Count = other.m_Count;
            delete[] m_Data;
            m_Data = new T[m_Allocated]{};
            std::copy(other.begin(), other.end(), m_Data);
            return *this;
        }

        TArray& operator=(TArray&& other) noexcept
        {
            if(this == &other)
                return *this;

            m_Data = other.m_Data;
            m_Count = other.m_Count;
            m_Allocated = other.m_Allocated;

            other.m_Data = nullptr;
            other.m_Count = 0;
            other.m_Allocated = 0;
            return *this;
        }

        ReferenceType operator[](SizeType index)
        {
            assert(index <= m_Count && m_Count != 0, "Index out of bounds");
            return m_Data[index];
        }

        ConstReferenceType operator[](SizeType index) const
        {
            assert(index <= m_Count && m_Count != 0, "Index out of bounds");
            return m_Data[index];
        }

        void setAt(SizeType index, ConstReferenceType element)
        {
            assert(index <= m_Count && m_Count != 0, "Index out of bounds");
            m_Data[index] = element;
        }

        ReferenceType getAt(SizeType index)
        {
            assert(index <= m_Count && m_Count != 0, "Index out of bounds");
            return m_Data[index];
        }

        ConstReferenceType getAt(SizeType index) const
        {
            assert(index <= m_Count && m_Count != 0, "Index out of bounds");
            return m_Data[index];
        }

        ReferenceType first()
        {
            assert(m_Count != 0, "Cannot get first element, array is empty!");
            return m_Data[0];
        }

        ConstReferenceType first() const
        {
            assert(m_Count != 0, "Cannot get first element, array is empty!");
            return m_Data[0];
        }

        ReferenceType last()
        {
            assert(m_Count != 0, "Cannot get last element, array is empty!");
            return m_Data[m_Count - 1];
        }

        ConstReferenceType last() const
        {
            assert(m_Count != 0, "Cannot get first element, array is empty!");
            return m_Data[m_Count - 1];
        }

        void add(ConstReferenceType element)
        {
            if(m_Count >= m_Allocated)
            {
                m_Allocated = getNewSize(m_Allocated);
                PointerType realloc = new T[m_Allocated]{};
                for(SizeType i = 0; i < m_Count; ++i)
                    realloc[i] = m_Data[i];
                delete[] m_Data;
                m_Data = realloc;
            }

            m_Data[m_Count++] = element;
        }

        void addUnique(ConstReferenceType element)
        {
            if (find(element) == -1)
                add(element);
        }

        template<typename... Args>
        void emplace(Args&&... args)
        {
            if(m_Count >= m_Allocated)
            {
                m_Allocated = getNewSize(m_Allocated);
                PointerType realloc = new T[m_Allocated]{};
                for(SizeType i = 0; i < m_Count; ++i)
                    realloc[i] = m_Data[i];
                delete[] m_Data;
                m_Data = realloc;
            }

            m_Data[m_Count++] = T(std::forward<Args>(args)...);
        }

        void addRange(const std::initializer_list<T>& list)
        {
            const SizeType totalCount = m_Count + list.size();
            if(totalCount >= m_Allocated)
            {
                do
                {
                    m_Allocated = getNewSize(m_Allocated);
                } while (m_Allocated < totalCount);
                PointerType newData = new T[m_Allocated]{};
                for(SizeType i = 0; i < m_Count; ++i)
                    newData[i] = m_Data[i];
                delete[] m_Data;
                m_Data = newData;
            }

            std::copy(list.begin(), list.end(), &m_Data[m_Count]);
            m_Count += list.size();
        }

        void addRange(const TArray& other)
        {
            const SizeType totalCount = m_Count + other.m_Count;
            if(totalCount >= m_Allocated)
            {
                do
                {
                    m_Allocated = getNewSize(m_Allocated);
                } while (m_Allocated < totalCount);

                PointerType newData = new T[m_Allocated]{};
                for(SizeType i = 0; i < m_Count; ++i)
                    newData[i] = m_Data[i];
                delete[] m_Data;
                m_Data = newData;
            }

            std::copy(other.begin(), other.end(), &m_Data[m_Count]);
            m_Count += other.count();
        }

        template<size_t N>
        void addRange(const T(&data)[N])
        {
            if (!data) return;

            const SizeType totalCount = m_Count + N;
            if(totalCount >= m_Allocated)
            {
                do
                {
                    m_Allocated = getNewSize(m_Allocated);
                } while (m_Allocated < totalCount);

                PointerType realloc = new T[m_Allocated]{};
                for(SizeType i = 0; i < m_Count; ++i)
                    realloc[i] = m_Data[i];
                delete[] m_Data;
                m_Data = realloc;
            }

            std::copy(data, data + N, &m_Data[m_Count]);
            m_Count += N;
        }

        void addRange(ConstPointerType data, SizeType count)
        {
            if (!data || count <= 0) return;

            const SizeType totalCount = m_Count + count;
            if(totalCount >= m_Allocated)
            {
                do
                {
                    m_Allocated = getNewSize(m_Allocated);
                } while (m_Allocated < totalCount);
                PointerType newData = new T[m_Allocated]{};
                for(SizeType i = 0; i < m_Count; ++i)
                    newData[i] = m_Data[i];
                delete[] m_Data;
                m_Data = newData;
            }

            std::copy(data, data + count, &m_Data[m_Count]);
            m_Count += count;
        }

        void emplace(T&& element)
        {
            if(m_Count >= m_Allocated)
            {
                m_Allocated = getNewSize(m_Allocated);
                PointerType newData = new T[m_Allocated]{};
                for(SizeType i = 0; i < m_Count; ++i)
                    newData[i] = m_Data[i];
                delete[] m_Data;
                m_Data = newData;
            }

            m_Data[m_Count++] = std::move(element);
        }

        TArray Union(const TArray& other)
        {
            TArray result = *this;
            for (const T& element : other)
                AddUnique(element);
            return result;
        }

        bool remove(ConstReferenceType element)
        {
            SizeType index = find(element);
            if(index == SizeType(-1)) return false;

            std::move(m_Data + index + 1, m_Data + m_Count, m_Data + index);
            m_Count--;
            return true;
        }

        void removeAt(SizeType index)
        {
            assert(index <= m_Count, "Index out of bounds!");
            std::move(m_Data + index + 1, m_Data + m_Count, m_Data + index);
            m_Count--;
        }

        bool removeAll(ConstReferenceType element)
        {
            SizeType index;
            bool found = false;
            while ((index = find(element)) != SizeType(-1))
            {
                if(index < m_Count) std::move(m_Data + index + 1, m_Data + m_Count, m_Data + index);
                m_Count--;
                found = true;
            }
            return found;
        }

        bool contains(ConstReferenceType element) const
        {
            for(SizeType i = 0; i < m_Count; ++i)
                if(m_Data[i] == element)
                    return true;
            return false;
        }

        SizeType find(ConstReferenceType element) const
        {
            for(SizeType i = 0; i < m_Count; ++i)
                if(m_Data[i] == element)
                    return i;
            return -1;
        }

        void popBack()
        {
            removeAt(m_Count);
        }

        void popHead()
        {
            removeAt(0);
        }

        void clear()
        {
            m_Count = 0;
        }

        void free()
        {
            delete[] m_Data;
            m_Data = nullptr;
            m_Count = 0;
            m_Allocated = 0;
        }

        // Return an array of pointer to elements of type T, inside m_Data, where each element satisfies Predicate
        TArray<PointerType> where(const std::function<bool(const T&)>& predicate) const
        {
            if(!predicate) return {};
            TArray<PointerType> Result;
            for(SizeType i = 0; i < m_Count; ++i)
            {
                if(predicate(m_Data[i]))
                    Result.add(&m_Data[i]);
            }
            return Result;
        }

        // Return a pointer to first elements of type T, inside m_Data, which satisfy Predicate
        PointerType single(const std::function<bool(const T&)>& predicate) const
        {
            if(!predicate) return nullptr;
            for(SizeType i = 0; i < m_Count; ++i)
            {
                if(predicate(m_Data[i]))
                    return &m_Data[i];
            }
            return nullptr;
        }

        template<typename Out>
        TArray<Out*> select(const std::function<Out*(T&)>& selector) const
        {
            if(!selector) return {};
            TArray<Out*> Result;
            for(SizeType i = 0; i < m_Count; ++i)
            {
                Result.add(selector(m_Data[i]));
            }
            return Result;
        }

        bool all(const std::function<bool(const T&)>& predicate) const
        {
            for (SizeType i = 0; i < m_Count; ++i)
            {
                if (!predicate(m_Data[i]))
                    return false;
            }
            return true;
        }

        bool any(const std::function<bool(const T&)>& predicate) const
        {
            for (SizeType i = 0; i < m_Count; ++i)
            {
                if (predicate(m_Data[i]))
                    return true;
            }
            return false;
        }

        bool isEmpty() const { return m_Count == 0; }


        template<typename U>
        TArray<U> transform(const std::function<U(const T&)>& predicate) const
        {
            TArray<U> result;
            for (size_t index = 0; index < m_Count; ++index)
            {
                const T& element = m_Data[index];
                result.add(predicate(element));
            }
            return result;
        }

        template<typename U> requires std::is_convertible_v<T, U>
        TArray<U> as()
        {
            return transform<U>([](const T& element) { return static_cast<U>(element); });
        }

        PointerType begin() { return m_Data; }
        PointerType end() { return m_Data + m_Count; }

        ConstPointerType begin() const { return m_Data; }
        ConstPointerType end() const { return m_Data + m_Count; }

        ConstPointerType data() const { return m_Data; }
        PointerType data() { return m_Data; }

        SizeType count() const { return m_Count; }
        SizeType size() const { return m_Count * sizeof(T); }

        bool operator==(const TArray& other) const
        {
            if(m_Count != other.m_Count) return false;
            for(SizeType i = 0; i < m_Count; ++i)
                if(m_Data[i] != other.m_Data[i]) return false;
            return true;
        }


        void sort(const std::function<bool(const T&, const T&)>& compareFunc)
        {
            if (m_Count <= 1)
                return;

            QuickSort(0, m_Count - 1, compareFunc);
        }
    private:
        void quickSort(SizeType low, SizeType high, const std::function<bool(const T&, const T&)>& compareFunc)
        {
            if (low >= high)
                return;

            SizeType pivotIndex = partition(low, high, compareFunc);

            if (pivotIndex > 0)
                quickSort(low, pivotIndex - 1, compareFunc);

            quickSort(pivotIndex + 1, high, compareFunc);
        }

        SizeType partition(SizeType low, SizeType high, const std::function<bool(const T&, const T&)>& compareFunc)
        {
            auto& pivot = m_Data[high];
            SizeType i = low;

            for (SizeType j = low; j < high; ++j)
            {
                if (compareFunc(m_Data[j], pivot))
                {
                    std::swap(m_Data[i], m_Data[j]);
                    ++i;
                }
            }

            std::swap(m_Data[i], m_Data[high]);
            return i;
        }
        
        SizeType getNewSize(SizeType current){ return 2 * current; }
        
        PointerType m_Data = nullptr;
        SizeType m_Count = 0;
        SizeType m_Allocated = 0;
    };
}
