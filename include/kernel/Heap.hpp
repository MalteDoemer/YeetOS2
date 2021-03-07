#pragma once

#include "stdint.h"
#include "stddef.h"

#define HEAP_MAGIC 0x69367420

struct HeapBlock {
    uint32_t magic;
    HeapBlock* prev;
    HeapBlock* next;
    size_t size;
    bool allocated;
};

class Heap {
private:
    HeapBlock* start;
    size_t size;

public:
    Heap(uintptr_t start, uintptr_t end);
    Heap();

    void* alloc(size_t size);
    void* realloc(void* mem, size_t size);
    void free(void* mem);
};

extern Heap kheap;
void init_heap();