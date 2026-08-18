#include "Luma/Memory/Memory.h"
#include <cstdlib>
#include <cstring>

namespace Luma::Memory
{
    void* allocate(size_t size)
    {
        return malloc(size);
    }

    void* reallocate(void* ptr, size_t size)
    {
        return realloc(ptr, size);
    }

    void free(void* ptr)
    {
        ::free(ptr);
    }

    void* callocate(size_t size)
    {
        return ::calloc(size, size);
    }

    void* alignedAllocate(size_t size, size_t alignment)
    {
#ifdef LUMA_PLATFORM_WINDOWS
        return _aligned_malloc(size, alignment);
#else
        return ::aligned_alloc(size, alignment);
#endif
    }

    void* alignedReallocate(void* ptr, size_t size, size_t alignment)
    {
#ifdef LUMA_PLATFORM_WINDOWS
        return _aligned_realloc(ptr, size, alignment);
#else
        return ::aligned_realloc(ptr, size, alignment);
#endif
    }

    void alignedFree(void* ptr)
    {
#ifdef LUMA_PLATFORM_WINDOWS
        _aligned_free(ptr);
#else
        ::aligned_free(ptr);
#endif
    }

    void* memset(void* ptr, int value, size_t size)
    {
        return ::memset(ptr, value, size);
    }

    void* memcpy(void* dest, const void* src, size_t size)
    {
        return ::memcpy(dest, src, size);
    }

    void* memmove(void* dest, const void* src, size_t size)
    {
        return ::memmove(dest, src, size);
    }
}
