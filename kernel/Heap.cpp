#include "YeetOS.h"
#include "Heap.h"

Heap::Heap(uintptr_t start, uintptr_t end)
{
    this->size = end - start;
    this->start = (HeapBlock*)start;

    this->start->magic = HEAP_MAGIC;
    this->start->size = size - sizeof(HeapBlock);
    this->start->allocated = false;
    this->start->prev = nullptr;
    this->start->next = nullptr;
}

Heap::Heap()
{
    
}

void* Heap::alloc(size_t size)
{
    HeapBlock* block = start;

    while (block && block->allocated)
        block = block->next;

    if (!block)
        return nullptr;

    if (block->size > size + sizeof(HeapBlock)) {
        HeapBlock* temp = (HeapBlock*)((uintptr_t)block + sizeof(HeapBlock) + size);

        temp->allocated = false;
        temp->magic = HEAP_MAGIC;
        temp->size = block->size - size - sizeof(HeapBlock);

        HeapBlock* next = block->next;
        if (next)
            next->prev = temp;

        block->next = temp;
        temp->prev = block;
    }

    block->allocated = true;

    return (void*)((uintptr_t)block + sizeof(HeapBlock));
}

void* Heap::realloc(void* mem, size_t size)
{
    free(mem);
    return alloc(size);
}

void Heap::free(void* mem)
{
    uintptr_t start_ptr = (uintptr_t)this->start;
    uintptr_t block_ptr = ((uintptr_t)mem - sizeof(HeapBlock));
    HeapBlock* block = (HeapBlock*)block_ptr;

    if (block_ptr < start_ptr || block_ptr < start_ptr + this->size)
        return;

    if (block->magic != HEAP_MAGIC)
        return;

    block->allocated = false;

    if (block->prev && !block->prev->allocated) {
        block->prev->next = block->next;
        block->prev->size += block->size + sizeof(HeapBlock);

        if (block->next)
            block->next->prev = block->prev;

        block = block->prev;
    }

    if (block->next && !block->next->allocated) {
        block->size += block->next->size + sizeof(HeapBlock);
        block->next = block->next->next;

        if (block->next)
            block->next->prev = block;
    }
}

void Heap::initialize()
{
    extern char heap_start;
    extern char heap_end;

    uintptr_t start = (uintptr_t)&heap_start;
    uintptr_t end = (uintptr_t)&heap_end;

    Heap::kheap = Heap(start, end);
}

Heap Heap::kheap;

