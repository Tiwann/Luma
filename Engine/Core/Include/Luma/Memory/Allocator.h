#pragma once
#include "Luma/Core/Export.h"

namespace Luma
{
    struct IAllocator
    {
        virtual ~IAllocator() = default;
        virtual void* allocate(size_t size, size_t alignment) = 0;
        virtual void* callocate(size_t size, size_t alignment) = 0;
        virtual void* reallocate(void* ptr, size_t size, size_t alignment) = 0;
        virtual void free(void* ptr) = 0;
    };


    struct LUMA_CORE_API FDefaultAllocator : IAllocator
    {
        void* allocate(size_t size, size_t alignment) override;
        void* callocate(size_t size, size_t alignment) override;
        void* reallocate(void* ptr, size_t size, size_t alignment) override;
        void free(void* ptr) override;
    };
}