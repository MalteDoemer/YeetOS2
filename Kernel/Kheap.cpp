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

#include "Kernel/New.hpp"
#include "Kernel/Kernel.hpp"
#include "Kernel/Kheap.hpp"
#include "Kernel/SlabAllocator.hpp"

namespace Kernel::Kheap {

SECTION(".heap_memory")
static Uint8 eternal_memory[4 * MiB];

static FlatPtr eternal_ptr = (FlatPtr)&eternal_memory;
static constexpr FlatPtr eternal_end = (FlatPtr)&eternal_memory + sizeof(eternal_memory);

static SlabAllocator<8, 512> slab_8;
static SlabAllocator<16, 512> slab_16;
static SlabAllocator<32, 512> slab_32;
static SlabAllocator<64, 512> slab_64;
static SlabAllocator<128, 512> slab_128;
static SlabAllocator<256, 512> slab_256;
static SlabAllocator<512, 512> slab_512;
static SlabAllocator<1024, 512> slab_1024;
static SlabAllocator<2048, 512> slab_2048;
static SlabAllocator<4096, 512> slab_4096;

void initialize()
{
    // call the constructors with placement new since
    // the kheap is initialized before all other
    // constructors are called

#ifndef VSCODE_IS_STUPID
    new (&slab_8) SlabAllocator<8, 512>();
    new (&slab_16) SlabAllocator<16, 512>();
    new (&slab_32) SlabAllocator<32, 512>();
    new (&slab_64) SlabAllocator<64, 512>();
    new (&slab_128) SlabAllocator<128, 512>();
    new (&slab_256) SlabAllocator<256, 512>();
    new (&slab_512) SlabAllocator<512, 512>();
    new (&slab_1024) SlabAllocator<1024, 512>();
    new (&slab_2048) SlabAllocator<2048, 512>();
    new (&slab_4096) SlabAllocator<4096, 512>();
#endif
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

void* allocate(size_t size)
{
    size = ALIGN(size, MIN_SLAB_SIZE);
    size = round_up_to_next_power_of_to(size);

    switch (size) {
    case 8:
        return slab_8.allocate();
    case 16:
        return slab_16.allocate();
    case 32:
        return slab_32.allocate();
    case 64:
        return slab_64.allocate();
    case 128:
        return slab_128.allocate();
    case 256:
        return slab_256.allocate();
    case 512:
        return slab_512.allocate();
    case 1024:
        return slab_1024.allocate();
    case 2048:
        return slab_2048.allocate();
    case 4096:
        return slab_4096.allocate();
    }

    // the kernel should never try to allocate more than 1 page
    VERIFY_NOT_REACHED();
}

void deallocate(void* ptr)
{

    if (slab_8.owns_ptr(ptr)) {
        slab_8.deallocate(ptr);
    } else if (slab_16.owns_ptr(ptr)) {
        slab_16.deallocate(ptr);
    } else if (slab_32.owns_ptr(ptr)) {
        slab_32.deallocate(ptr);
    } else if (slab_64.owns_ptr(ptr)) {
        slab_64.deallocate(ptr);
    } else if (slab_128.owns_ptr(ptr)) {
        slab_128.deallocate(ptr);
    } else if (slab_256.owns_ptr(ptr)) {
        slab_256.deallocate(ptr);
    } else if (slab_512.owns_ptr(ptr)) {
        slab_512.deallocate(ptr);
    } else if (slab_1024.owns_ptr(ptr)) {
        slab_1024.deallocate(ptr);
    } else if (slab_2048.owns_ptr(ptr)) {
        slab_2048.deallocate(ptr);
    } else if (slab_4096.owns_ptr(ptr)) {
        slab_4096.deallocate(ptr);
    }

    VERIFY_NOT_REACHED();
}

}