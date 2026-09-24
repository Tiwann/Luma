#pragma once
#include "Array.h"

namespace Luma
{
    template <typename T>
    class Queue
    {
    public:
        using ArrayType = Array<T>;
        using SizeType = ArrayType::SizeType;
        using ValueType = ArrayType::ValueType;
        using ReferenceType = ArrayType::ReferenceType;
        using ConstReferenceType = ArrayType::ConstReferenceType;
        using ForwardReferenceType = ArrayType::ForwardType;

        Queue() = default;
        Queue(const Queue&) = default;
        Queue(Queue&&) = default;
        Queue& operator=(const Queue&) = default;
        Queue& operator=(Queue&&) = default;
        
        void enqueue(ConstReferenceType item)
        {
            m_Data.add(item);
        }

        template<typename... Args>
        void enqueue(Args&&... args)
        {
            m_Data.emplace(args...);
        }

        ValueType dequeue()
        {
            ForwardReferenceType first = std::move(m_Data.first());
            m_Data.popHead();
            return first;
        }

        bool isEmpty() const { return m_Data.isEmpty(); }

        void clear()
        {
            m_Data.clear();
        }

        void free()
        {
            m_Data.free();
        }
    private:
        ArrayType m_Data;
    };
}