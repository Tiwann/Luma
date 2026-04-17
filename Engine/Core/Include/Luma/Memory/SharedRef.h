#pragma once
#include "RefCounted.h"
#include <type_traits>

namespace Luma
{
    template <typename T>
    class TSharedRef
    {
    public:
        using PointerType = T*;
        using ConstPointerType = const T*;
        using ReferenceType = T&;
        using ConstReferenceType = const T&;
        using PointerToConst = T* const;
        using ConstPointerToConst = const T* const;
        using ValueType = T;

        TSharedRef() = default;

        explicit TSharedRef(PointerType ptr) : m_Pointer(ptr)
        {
            addRef(m_Pointer);
        }

        TSharedRef(decltype(nullptr)) : m_Pointer(nullptr) {}

        TSharedRef(const TSharedRef& other) : m_Pointer(other.m_Pointer)
        {
            addRef(m_Pointer);
        }

        TSharedRef(TSharedRef&& other) noexcept : m_Pointer(other.m_Pointer)
        {
            other.m_Pointer = nullptr;
        }

        template <typename U>
        TSharedRef(const TSharedRef<U>& other) : m_Pointer(reinterpret_cast<PointerType>(other.m_Pointer))
        {
            addRef(m_Pointer);
        }

        ~TSharedRef()
        {
            relRef(m_Pointer);
        }

        TSharedRef& operator=(const TSharedRef& other)
        {
            if (this == &other)
                return *this;

            PointerType old = m_Pointer;
            addRef(other.m_Pointer);
            m_Pointer = other.m_Pointer;
            relRef(old);
            return *this;
        }

        TSharedRef& operator=(TSharedRef&& other) noexcept
        {
            if (this == &other)
                return *this;

            relRef(m_Pointer);
            m_Pointer = other.m_Pointer;
            other.m_Pointer = nullptr;

            return *this;
        }

        template <typename U> requires (!std::is_same_v<T, U>)
        TSharedRef& operator=(const TSharedRef<U>& other)
        {
            PointerType old = m_Pointer;
            addRef(other.m_Pointer);
            m_Pointer = other.m_Pointer;
            relRef(old);
            return *this;
        }

        template <typename U> requires (!std::is_same_v<T, U>)
        TSharedRef& operator=(TSharedRef<U>&& other)
        {
            relRef(m_Pointer);
            m_Pointer = other.m_Pointer;
            other.m_Pointer = nullptr;
            return *this;
        }

        void operator=(decltype(nullptr))
        {
            relRef(m_Pointer);
            m_Pointer = nullptr;
        }

        bool operator==(const PointerType ptr) const { return m_Pointer == ptr; }
        bool operator!=(const PointerType ptr) const { return m_Pointer != ptr; }
        bool operator==(const TSharedRef& ptr) const { return m_Pointer == ptr.m_Pointer; }
        bool operator!=(const TSharedRef& ptr) const { return m_Pointer != ptr.m_Pointer; }

        template <typename U>
        TSharedRef<U> as() const
        {
            return TSharedRef<U>(dynamic_cast<U*>(m_Pointer));
        }


        ReferenceType operator*() { return *m_Pointer; }
        ConstReferenceType operator*() const { return *m_Pointer; }

        PointerType operator->() { return m_Pointer; }
        ConstPointerType operator->() const { return m_Pointer; }

        PointerType get() { return m_Pointer; }
        ConstPointerType get() const { return m_Pointer; }

        explicit operator PointerType() { return m_Pointer; }
        explicit operator ConstPointerType() const { return m_Pointer; }
        operator bool() const { return m_Pointer; }


        void attach(PointerType newPointer)
        {
            addRef(newPointer);
            relRef(m_Pointer);
            m_Pointer = newPointer;
        }

        PointerType detach()
        {
            const PointerType result = m_Pointer;
            m_Pointer = nullptr;
            return result;
        }

        void swap(TSharedRef& other) noexcept
        {
            PointerType otherPointer = other.m_Pointer;
            other.m_Pointer = m_Pointer;
            m_Pointer = otherPointer;
        }
    private:
        PointerType m_Pointer = nullptr;

        template <typename U>
        friend class TSharedRef;

        template <typename U>
        friend class TWeakRef;
    };

    template<typename T>
    class TWeakRef
    {
    public:
        using PointerType = TSharedRef<T>::PointerType;
        using ConstPointerType = TSharedRef<T>::ConstPointerType;
        using ReferenceType = TSharedRef<T>::ReferenceType;
        using ConstReferenceType = TSharedRef<T>::ConstReferenceType;

        TWeakRef(decltype(nullptr)) : m_Pointer(nullptr) {}
        TWeakRef(const TSharedRef<T>& ref) : m_Pointer(ref.m_Pointer) {}
        TWeakRef(const TWeakRef&) = default;
        TWeakRef(TWeakRef&&) = default;
        TWeakRef& operator=(const TWeakRef&) = default;
        TWeakRef& operator=(TWeakRef&&) = default;
        ~TWeakRef() = default;

        explicit operator PointerType() { return m_Pointer; }
        explicit operator ConstPointerType() const { return m_Pointer; }
        operator bool() const { return m_Pointer; }
        
        ReferenceType operator*() { return *m_Pointer; }
        ConstReferenceType operator*() const { return *m_Pointer; }

        PointerType operator->() { return m_Pointer; }
        ConstPointerType operator->() const { return m_Pointer; }

        PointerType get() { return m_Pointer; }
        ConstPointerType get() const { return m_Pointer; }
    private:
        PointerType m_Pointer = nullptr;
    };

    template <typename T, typename... Args> requires std::is_base_of_v<IRefCounted<T>, T>
    TSharedRef<T> createSharedRef(Args&&... args)
    {
        return TSharedRef<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T> requires std::is_base_of_v<IRefCounted<T>, T>
    TWeakRef<T> createWeakRef(const TSharedRef<T>& ref) { return TWeakRef<T>(ref); }
}
