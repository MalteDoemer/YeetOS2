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

#include "stddef.h"
#include "Kernel/Heap.hpp"
#include "Kernel/Kheap.hpp"
#include "Kernel/KernelTests.hpp"

namespace Kernel::Tests {

bool test_kheap_total_size()
{
    Heap& kheap = Kheap::get_kheap();

    size_t expected_heap_size = kheap.m_end - kheap.m_start - 3 * HeapBlock::size();
    size_t actual_heap_size = kheap.m_total_free + kheap.m_total_used;

    return expected_heap_size == actual_heap_size;
}

bool test_kheap_size_after_allocation_and_deallocation()
{
    Heap& kheap = Kheap::get_kheap();

    for (size_t i = kheap.min_alloc_size; i < 8 * KiB; i += kheap.min_alloc_size) {

        size_t free_size = kheap.m_total_free;
        size_t used_size = kheap.m_total_used;

        void* data = Kheap::allocate(i);

        if (free_size != kheap.m_total_free + i || used_size + i != kheap.m_total_used)
            return false;

        Kheap::deallocate(data);

        if (free_size != kheap.m_total_free || used_size != kheap.m_total_used)
            return false;
    }

    return true;
}

bool test_kheap_coalescing()
{
    Heap& kheap = Kheap::get_kheap();
    size_t free_before_alloc = kheap.m_total_free;

    constexpr auto num_allocs = 512;
    constexpr auto alloc_size = 1 * KiB;

    void* ptrs[num_allocs];

    for (size_t i = 0; i < num_allocs; i++) {
        ptrs[i] = Kheap::allocate(alloc_size);
    }

    for (size_t i = 0; i < num_allocs; i += 2) {
        Kheap::deallocate(ptrs[i]);
    }

    for (size_t i = 1; i < num_allocs; i += 2) {
        Kheap::deallocate(ptrs[i]);
    }

    return free_before_alloc == kheap.m_total_free;
}

TEST_FUNCTION(test_kheap_total_size);
TEST_FUNCTION(test_kheap_size_after_allocation_and_deallocation);
TEST_FUNCTION(test_kheap_coalescing);

};