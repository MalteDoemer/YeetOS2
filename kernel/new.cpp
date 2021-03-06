
#include "YeetOS.h"
#include "Heap.h"

void* operator new(size_t size)
{
    return Heap::kheap.alloc(size);
}

void* operator new[](size_t size)
{
    return Heap::kheap.alloc(size);
}

void operator delete(void* ptr)
{
    Heap::kheap.free(ptr);
}

void operator delete[](void* ptr)
{
    Heap::kheap.free(ptr);
}
