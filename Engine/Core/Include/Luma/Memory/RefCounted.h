#pragma once
#include <atomic>

namespace Luma
{
    template<typename T>
    struct IRefCounted
    {
        IRefCounted() = default;
        virtual ~IRefCounted() = default;

        void addRef()
        {
            refCount.fetch_add(1, std::memory_order_acquire);
        }

        void relRef()
        {
            if (refCount.fetch_sub(1, std::memory_order_release) == 1)
            {
                T* self = reinterpret_cast<T*>(this);
                self->destroy();
                delete self;
            }
        }
    protected:
        mutable std::atomic<uint32_t> refCount{0};
    };

    template<typename T>
    void addRef(T* ptr)
    {
        if (ptr) ptr->addRef();
    }

    template<typename T>
    void relRef(T* ptr)
    {
        if (ptr) ptr->relRef();
    }
}