#include "Luma/Memory/Allocator.h"
#include "Luma/Memory/Memory.h"
#include <new>

namespace Luma
{
    void* FDefaultAllocator::allocate(size_t size, size_t alignment)
    {
        return ::operator new(size);
    }

    void* FDefaultAllocator::callocate(size_t size, size_t alignment)
    {
        void* data = ::operator new(size);
        Memory::memset(data, 0, size);
        return data;
    }

    void* FDefaultAllocator::reallocate(void* ptr, size_t size, size_t alignment)
    {
        return ::operator new (size, ptr);
    }

    void FDefaultAllocator::free(void* ptr)
    {
        ::operator delete(ptr);
    }
}
