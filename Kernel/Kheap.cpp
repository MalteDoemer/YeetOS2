/*
 * Copyright 2021 Malte Dömer
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "Platform.hpp"
#include "Assertions.hpp"
#include "New.hpp"

#include "Kernel/Panic.hpp"
#include "Kernel/Kernel.hpp"
#include "Kernel/Heap.hpp"
#include "Kernel/Kheap.hpp"

namespace Kernel::Kheap {

SECTION(".heap_memory")
static Uint8 eternal_memory[4 * MiB];

static FlatPtr eternal_ptr = (FlatPtr)&eternal_memory;
static constexpr FlatPtr eternal_end = (FlatPtr)&eternal_memory + sizeof(eternal_memory);

// default constructor of global_heap should never run
static Heap global_heap;

void initialize()
{
    // call the constructors with placement new since
    // the kheap is initialized before all other
    // constructors are called

    new (&global_heap) Heap(alloc_eternal(1 * MiB, 1), 1 * MiB);
}

void* alloc_eternal(size_t size, size_t alignment)
{
    eternal_ptr = ALIGN(eternal_ptr, alignment);
    FlatPtr ret = eternal_ptr;
    eternal_ptr += size;

    if (eternal_ptr >= eternal_end) [[unlikely]]
        panic("Kernel out of eternal memory!");

    return (void*)ret;
}

FLATTEN void* allocate(size_t size)
{
    return global_heap.allocate(size);
}

FLATTEN void* reallocate(void* ptr, size_t size)
{
    return global_heap.reallocate(ptr, size);
}

FLATTEN void deallocate(void* ptr)
{
    global_heap.deallocate(ptr);
}

}

FLATTEN void* operator new(size_t size)
{
    return Kernel::Kheap::allocate(size);
}

FLATTEN void* operator new[](size_t size)
{
    return Kernel::Kheap::allocate(size);
}

FLATTEN void operator delete(void* ptr, size_t size)
{
    Kernel::Kheap::deallocate(ptr);
}

FLATTEN void operator delete[](void* ptr, size_t size)
{
    Kernel::Kheap::deallocate(ptr);
}

#ifdef __KERNEL_TESTS__

namespace Kernel::Kheap {

Heap& get_kheap()
{
    return global_heap;
}

}
#endif