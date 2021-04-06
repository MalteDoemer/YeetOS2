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

#pragma once

#include "Types.hpp"
#include "InlineBitmap.hpp"

#include "Kernel/Kheap.hpp"

#define MIN_SLAB_SIZE 8
#define MAX_SLAB_SIZE 4096

namespace Kernel {

template<size_t slab_size, size_t slab_count>
requires requires
{
    requires(slab_count > 0);
    requires(slab_count % 32 == 0);
    requires(slab_size >= MIN_SLAB_SIZE);
    requires(slab_size <= MAX_SLAB_SIZE);
}
class SlabAllocator {

public:
    SlabAllocator()
    {
        m_memory = (FlatPtr)Kheap::alloc_eternal(slab_count * slab_size, slab_size);
    }

    void* allocate()
    {
        for (size_t i = m_last_alloc; i < m_bitmap.count(); i++) {
            if (!m_bitmap.get_bit(i)) {
                m_last_alloc = i + 1;
                m_bitmap.set_bit(i);
                return index_to_ptr(i);
            }
        }

        // TODO: grow the heap when it needs to

        return nullptr;
    }

    void deallocate(void* ptr)
    {
        FlatPtr fptr = (FlatPtr)ptr;
        size_t index = ptr_to_index(ptr);

        
        // Verify that deallocate is never called with a pointer that
        // isn't owned by us, a pointer that is missaligned or a 
        // pointer that was already deallocated
        // These will be optimized away if NDEBUG is defined so
        // deallocate will be pretty fast

        VERIFY(owns_ptr(ptr));
        VERIFY(fptr == ALIGN(fptr, slab_size));
        VERIFY(m_bitmap.get_bit(index));

        m_bitmap.clear_bit(ptr_to_index(ptr));
        
        if (index < m_last_alloc)
            m_last_alloc = index;
    }

public:
    ALWAYS_INLINE constexpr bool owns_ptr(void* ptr)
    {
        FlatPtr fptr = (FlatPtr)ptr;
        return fptr >= m_memory && fptr < (m_memory + slab_count * slab_size);
    }

private:
    ALWAYS_INLINE constexpr void* index_to_ptr(size_t index)
    {
        return (void*)(m_memory + index * slab_size);
    }

    ALWAYS_INLINE constexpr size_t ptr_to_index(void* ptr)
    {
        return ((FlatPtr)ptr - m_memory) / slab_size;
    }

private:
    InlineBitmap<slab_count, Uint32> m_bitmap;
    size_t m_last_alloc { 0 };
    FlatPtr m_memory { 0 };
};

}