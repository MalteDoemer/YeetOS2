
#include "kernel/YeetOS.hpp"
#include "kernel/Heap.hpp"

void* operator new(size_t size)
{
    return kheap.alloc(size);
}

void* operator new[](size_t size)
{
    return kheap.alloc(size);
}

void operator delete(void* ptr, size_t size)
{
    kheap.free(ptr);
}

void operator delete[](void* ptr)
{
    kheap.free(ptr);
}
