
#include "kernel/YeetOS.hpp"
#include "kernel/Heap.hpp"

void* operator new(size_t size)
{
    return Heap::kheap.alloc(size);
}

void* operator new[](size_t size)
{
    return Heap::kheap.alloc(size);
}

void operator delete(void* ptr, size_t size)
{
    Heap::kheap.free(ptr);
}

void operator delete[](void* ptr)
{
    Heap::kheap.free(ptr);
}
