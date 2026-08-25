#pragma once
#include "Luma/Core/Export.h"
#include <cstdint>
#include <cstddef>

namespace Luma::Memory
{
    LUMA_CORE_API void* allocate(uint64_t size);
    LUMA_CORE_API void* reallocate(void* ptr, uint64_t size);
    LUMA_CORE_API void free(void* ptr);
    LUMA_CORE_API void* callocate(uint64_t size);
    LUMA_CORE_API void* alignedAllocate(uint64_t size, uint64_t alignment);
    LUMA_CORE_API void* alignedReallocate(void* ptr, uint64_t size, uint64_t alignment);
    LUMA_CORE_API void alignedFree(void* ptr);
    LUMA_CORE_API void* memset(void* ptr, int value, uint64_t size);
    LUMA_CORE_API void* memcpy(void* dest, const void* src, uint64_t size);
    LUMA_CORE_API void* memmove(void* dest, const void* src, uint64_t size);
    LUMA_CORE_API constexpr uint64_t alignUp(uint64_t size, uint64_t alignment) { return (size + alignment - 1) & ~(alignment - 1); }
    LUMA_CORE_API constexpr uint64_t KB(uint32_t n) { return n * 1024; }
    LUMA_CORE_API constexpr uint64_t MB(uint32_t n) { return n * 1024 * 1024; }
}