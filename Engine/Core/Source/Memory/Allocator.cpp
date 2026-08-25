#include "Luma/Memory/Allocator.h"
#include "Luma/Memory/Memory.h"
#include <new>

namespace Luma
{
    void* FDefaultAllocator::allocate(uint64_t size, uint64_t alignment)
    {
        return ::operator new(size);
    }

    void* FDefaultAllocator::callocate(uint64_t size, uint64_t alignment)
    {
        void* data = ::operator new(size);
        Memory::memset(data, 0, size);
        return data;
    }

    void* FDefaultAllocator::reallocate(void* ptr, uint64_t size, uint64_t alignment)
    {
        return ::operator new (size, ptr);
    }

    void FDefaultAllocator::free(void* ptr)
    {
        ::operator delete(ptr);
    }
}
