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
    struct IRefCountedBase
    {
        IRefCountedBase() = default;
        virtual ~IRefCountedBase() = default;

        void addRef()
        {
            m_RefCount.fetch_add(1, std::memory_order_relaxed);
        }

        void relRef()
        {
            if (m_RefCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
            {
                // Call virtual destroy on most-derived object then delete
                destroy();
                delete this;
            }
        }

        uint32_t getRefCount() const { return m_RefCount.load(); }

        // Must be implemented by most-derived type (or its base) to release resources
        virtual void destroy() = 0;
    private:
        mutable std::atomic<uint32_t> m_RefCount{0};
    };

    template<typename T>
    struct IRefCounted : IRefCountedBase
    {
        IRefCounted() = default;
        virtual ~IRefCounted() = default;
    };

    template<typename T>
    void addRef(T* ptr)
    {
        if (ptr) reinterpret_cast<IRefCountedBase*>(ptr)->addRef();
    }

    template<typename T>
    void relRef(T* ptr)
    {
        if (ptr) reinterpret_cast<IRefCountedBase*>(ptr)->relRef();
    }
}