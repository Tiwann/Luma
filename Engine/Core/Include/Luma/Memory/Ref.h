#pragma once
#include "RefCounted.h"
#include <type_traits>

namespace Luma
{
    template <typename T>
    class Ref
    {
    public:
        using PointerType = T*;
        using ConstPointerType = const T*;
        using ReferenceType = T&;
        using ConstReferenceType = const T&;
        using PointerToConst = T* const;
        using ConstPointerToConst = const T* const;
        using ValueType = T;

        Ref() = default;

        explicit Ref(PointerType ptr) : m_Pointer(ptr)
        {
            addRef(m_Pointer);
        }

        Ref(decltype(nullptr)) : m_Pointer(nullptr) {}

        Ref(const Ref& other) : m_Pointer(other.m_Pointer)
        {
            addRef(m_Pointer);
        }

        Ref(Ref&& other) noexcept : m_Pointer(other.m_Pointer)
        {
            other.m_Pointer = nullptr;
        }

        template <typename U>
        Ref(const Ref<U>& other) : m_Pointer(reinterpret_cast<PointerType>(other.m_Pointer))
        {
            addRef(m_Pointer);
        }

        ~Ref()
        {
            relRef(m_Pointer);
        }

        Ref& operator=(const Ref& other)
        {
            if (this == &other)
                return *this;

            PointerType old = m_Pointer;
            addRef(other.m_Pointer);
            m_Pointer = other.m_Pointer;
            relRef(old);
            return *this;
        }

        Ref& operator=(Ref&& other) noexcept
        {
            if (this == &other)
                return *this;

            relRef(m_Pointer);
            m_Pointer = other.m_Pointer;
            other.m_Pointer = nullptr;

            return *this;
        }

        template <typename U> requires (!std::is_same_v<T, U>)
        Ref& operator=(const Ref<U>& other)
        {
            PointerType old = m_Pointer;
            addRef(other.m_Pointer);
            m_Pointer = other.m_Pointer;
            relRef(old);
            return *this;
        }

        template <typename U> requires (!std::is_same_v<T, U>)
        Ref& operator=(Ref<U>&& other)
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
        bool operator==(const Ref& ptr) const { return m_Pointer == ptr.m_Pointer; }
        bool operator!=(const Ref& ptr) const { return m_Pointer != ptr.m_Pointer; }

        template <typename U>
        Ref<U> as() const
        {
            return Ref<U>(dynamic_cast<U*>(m_Pointer));
        }


        ReferenceType operator*() { return *m_Pointer; }
        ConstReferenceType operator*() const { return *m_Pointer; }

        PointerType operator->() { return m_Pointer; }
        ConstPointerType operator->() const { return m_Pointer; }

        PointerType get() { return m_Pointer; }
        ConstPointerType get() const { return m_Pointer; }

        operator PointerType() { return m_Pointer; }
        operator ConstPointerType() const { return m_Pointer; }
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

        void swap(Ref& other) noexcept
        {
            PointerType otherPointer = other.m_Pointer;
            other.m_Pointer = m_Pointer;
            m_Pointer = otherPointer;
        }
    private:
        PointerType m_Pointer = nullptr;

        template <typename U>
        friend class Ref;

        template <typename U>
        friend class WeakRef;
    };

    template<typename T>
    class WeakRef
    {
    public:
        using PointerType = Ref<T>::PointerType;
        using ConstPointerType = Ref<T>::ConstPointerType;
        using ReferenceType = Ref<T>::ReferenceType;
        using ConstReferenceType = Ref<T>::ConstReferenceType;

        WeakRef(decltype(nullptr)) : m_Pointer(nullptr) {}
        WeakRef(const Ref<T>& ref) : m_Pointer(ref.m_Pointer) {}
        WeakRef(const WeakRef&) = default;
        WeakRef(WeakRef&&) = default;
        WeakRef& operator=(const WeakRef&) = default;
        WeakRef& operator=(WeakRef&&) = default;
        ~WeakRef() = default;

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
    Ref<T> createSharedRef(Args&&... args)
    {
        return Ref<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T> requires std::is_base_of_v<IRefCounted<T>, T>
    Ref<T> makeShared(T* object)
    {
        return Ref<T>(object);
    }

    template<typename T> requires std::is_base_of_v<IRefCounted<T>, T>
    WeakRef<T> createWeakRef(const Ref<T>& ref) { return WeakRef<T>(ref); }
}
