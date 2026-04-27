#pragma once
#include <atomic>

#define LUMA_NO_COPYABLE(className) \
    className& operator=(const className&) = delete; \
    className(const className&) = delete


#define LUMA_NO_MOVABLE(className) \
    className& operator=(className&&) = delete; \
    className(className&&) = delete


#define LUMA_NO_COPYMOVE(className) \
    LUMA_NO_COPYABLE(className); \
    LUMA_NO_MOVABLE(className)

namespace Luma
{
    template<typename T>
    struct IRefCounted
    {
        IRefCounted() = default;
        virtual ~IRefCounted() = default;

        void addRef()
        {
            m_RefCount.fetch_add(1, std::memory_order_acquire);
        }

        void relRef()
        {
            if (m_RefCount.fetch_sub(1, std::memory_order_release) == 1)
            {
                T* self = static_cast<T*>(this);
                self->destroy();
                delete self;
            }
        }
    private:
        mutable std::atomic<uint32_t> m_RefCount{0};
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