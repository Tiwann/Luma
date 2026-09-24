#include "Luma/Memory/Allocator.h"
#include "Luma/Memory/Memory.h"
#include <new>

namespace Luma
{
    void* LinearAllocator::allocate(uint64_t size, uint64_t alignment)
    {
        return ::operator new(size);
    }

    void* LinearAllocator::callocate(uint64_t size, uint64_t alignment)
    {
        void* data = ::operator new(size);
        Memory::memset(data, 0, size);
        return data;
    }

    void* LinearAllocator::reallocate(void* ptr, uint64_t size, uint64_t alignment)
    {
        return ::operator new (size, ptr);
    }

    void LinearAllocator::free(void* ptr)
    {
        ::operator delete(ptr);
    }
}
