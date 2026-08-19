#pragma once
#include "Luma/Core/Export.h"
#include <cstdint>
#include <cstddef>

namespace Luma::Memory
{
    LUMA_CORE_API void* allocate(size_t size);
    LUMA_CORE_API void* reallocate(void* ptr, size_t size);
    LUMA_CORE_API void free(void* ptr);
    LUMA_CORE_API void* callocate(size_t size);
    LUMA_CORE_API void* alignedAllocate(size_t size, size_t alignment);
    LUMA_CORE_API void* alignedReallocate(void* ptr, size_t size, size_t alignment);
    LUMA_CORE_API void alignedFree(void* ptr);
    LUMA_CORE_API void* memset(void* ptr, int value, size_t size);
    LUMA_CORE_API void* memcpy(void* dest, const void* src, size_t size);
    LUMA_CORE_API void* memmove(void* dest, const void* src, size_t size);
    LUMA_CORE_API constexpr uint64_t alignUp(uint64_t size, uint64_t alignment) { return (size + alignment - 1) & ~(alignment - 1); }
    LUMA_CORE_API constexpr uint64_t KB(uint32_t n) { return n * 1024; }
    LUMA_CORE_API constexpr uint64_t MB(uint32_t n) { return n * 1024 * 1024; }
}