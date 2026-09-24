#pragma once
#include "Luma/Core/Export.h"
#include <cstdint>

namespace Luma
{
    struct Allocator
    {
        virtual ~Allocator() = default;
        virtual void* allocate(uint64_t size, uint64_t alignment) = 0;
        virtual void* callocate(uint64_t size, uint64_t alignment) = 0;
        virtual void* reallocate(void* ptr, uint64_t size, uint64_t alignment) = 0;
        virtual void free(void* ptr) = 0;
    };


    struct LUMA_CORE_API LinearAllocator : Allocator
    {
        void* allocate(uint64_t size, uint64_t alignment) override;
        void* callocate(uint64_t size, uint64_t alignment) override;
        void* reallocate(void* ptr, uint64_t size, uint64_t alignment) override;
        void free(void* ptr) override;
    };
}