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
#include "Platform.hpp"
#include "StdLibExtras.hpp"
#include "SizeWithInfo.hpp"
#include "Kernel/Kernel.hpp"

struct PACKED HeapBlock {

    enum State {
        Free = 0,
        Used = 1,
        Special = 2,
    };

    ALWAYS_INLINE constexpr static size_t size() { return 2 * sizeof(size_t); }

    ALWAYS_INLINE void* data() { return m_data; }

    ALWAYS_INLINE HeapBlock* get_prev() const { return reinterpret_cast<HeapBlock*>(reinterpret_cast<FlatPtr>(this) - prev_size - size()); }
    ALWAYS_INLINE HeapBlock* get_next() const { return reinterpret_cast<HeapBlock*>(reinterpret_cast<FlatPtr>(this) + this_size + size()); }

    ALWAYS_INLINE HeapBlock* get_next_free() const { return next_free; }
    ALWAYS_INLINE void set_next_free(HeapBlock* next) { next_free = next; }

    ALWAYS_INLINE HeapBlock* get_prev_free() const { return prev_free; }
    ALWAYS_INLINE void set_prev_free(HeapBlock* prev) { prev_free = prev; }

    ALWAYS_INLINE State get_state() const { return static_cast<State>(this_size.info); }
    ALWAYS_INLINE void set_state(State state) { this_size.info = state; }

    ALWAYS_INLINE State get_prev_state() const { return static_cast<State>(prev_size.info); }
    ALWAYS_INLINE void set_prev_state(State state) { prev_size.info = state; }

    ALWAYS_INLINE size_t get_size() const { return this_size; }
    ALWAYS_INLINE void set_size(size_t size) { this_size = size; }

    ALWAYS_INLINE size_t get_prev_size() const { return prev_size; }
    ALWAYS_INLINE void set_prev_size(size_t size) { prev_size = size; }

private:
    SizeWithInfo<2> prev_size { 0 };
    SizeWithInfo<2> this_size { 0 };

    Uint8 m_data[0];
    HeapBlock* next_free { nullptr };
    HeapBlock* prev_free { nullptr };
};

static_assert(sizeof(HeapBlock) == HeapBlock::size() + 2 * sizeof(HeapBlock*));

class Heap {

public:
    constexpr static size_t num_buckets = 9;
    constexpr static size_t min_alloc_size = 16;
    constexpr static size_t first_bucket_log2 = 4;

    Heap()
    {
        VERIFY_NOT_REACHED();
    }

    Heap(void* memory, size_t size) :
        m_start((FlatPtr)memory), m_end((FlatPtr)memory + size)
    {

        HeapBlock* first_block = reinterpret_cast<HeapBlock*>(m_start);
        HeapBlock* main_block = reinterpret_cast<HeapBlock*>(m_start + HeapBlock::size());
        HeapBlock* last_block = reinterpret_cast<HeapBlock*>(m_end - HeapBlock::size());

        first_block->set_size(0);
        first_block->set_state(HeapBlock::State::Special);

        main_block->set_prev_size(0);
        main_block->set_prev_state(HeapBlock::State::Special);
        main_block->set_size(size - 3 * HeapBlock::size());
        main_block->set_state(HeapBlock::State::Free);

        last_block->set_prev_size(size - 3 * HeapBlock::size());
        last_block->set_prev_state(HeapBlock::State::Free);
        last_block->set_size(0);
        last_block->set_state(HeapBlock::State::Special);

        main_block->set_next_free(nullptr);
        main_block->set_prev_free(nullptr);
        size_t bucket = get_bucket_index(main_block->get_size());
        m_buckets[bucket] = main_block;
    }

    void* allocate(size_t size)
    {
        if (size == 0) [[unlikely]]
            return nullptr;

        size = ALIGN(size, min_alloc_size);

        size_t bucket = get_bucket_index(size);

        for (size_t i = bucket; i < num_buckets; i++) {
            for (HeapBlock* block = m_buckets[i]; block; block = block->get_next_free()) {

                if (block->get_size() >= size) {

                    /* First thing to do is remove the block from free list */
                    remove_from_free_list(i, block);

                    if (should_split(size, block->get_size())) {

                        /* Split the old block and insert a new one:

                        +--------------------+-------------+
                        | block              | next_block  |
                        +--------------------+-------------+

                        +--------+-----------+-------------+
                        | block  | new_block | next_block  |
                        +--------+-----------+-------------+ */

                        HeapBlock* next_block = block->get_next();
                        HeapBlock* new_block = reinterpret_cast<HeapBlock*>(reinterpret_cast<FlatPtr>(block) + size + HeapBlock::size());

                        new_block->set_size(block->get_size() - size - HeapBlock::size());
                        new_block->set_state(HeapBlock::State::Free);

                        next_block->set_prev_size(new_block->get_size());
                        next_block->set_prev_state(HeapBlock::State::Free);

                        block->set_size(size);
                        // block->set_state(HeapBlock::State::Used);

                        new_block->set_prev_size(size);
                        // new_block->set_prev_state(HeapBlock::State::Used);

                        /* Add new_block to the front of the free list */
                        size_t idx = get_bucket_index(new_block->get_size());
                        add_to_free_list(idx, new_block);
                    }

                    block->set_state(HeapBlock::State::Used);
                    block->get_next()->set_prev_state(HeapBlock::State::Used);

                    return block->data();
                }
            }
        }

        return nullptr;
    }

    void* reallocate(void* ptr, size_t size)
    {
        return nullptr;
    }

    void deallocate(void* ptr)
    {

        /* Allow to delete nullptr */
        if (!ptr)
            return;

#ifdef VERIFY_HEAP
        VERIFY(ptr == ALIGN(ptr, min_alloc_size));
        VERIFY(is_heap_ptr(ptr));
#endif
        HeapBlock* block = reinterpret_cast<HeapBlock*>(reinterpret_cast<FlatPtr>(ptr) - HeapBlock::size());

        /* next_free and prev_free must be reset since they were (most likely) overriden by the user */
        block->set_next_free(nullptr);
        block->set_prev_free(nullptr);

#ifdef VERIFY_HEAP
        VERIFY(block->get_state() != HeapBlock::State::Special);
        VERIFY(block->get_state() != HeapBlock::State::Free);
#endif

        if (block->get_prev_state() == HeapBlock::State::Free) {

            HeapBlock* prev_block = block->get_prev();
            HeapBlock* next_block = block->get_next();

            /* Remove prev_block from the free list */
            size_t prev_idx = get_bucket_index(prev_block->get_size());
            remove_from_free_list(prev_idx, prev_block);

            /* Merge block into prev_block */
            prev_block->set_size(prev_block->get_size() + block->get_size() + HeapBlock::size());
            next_block->set_prev_size(prev_block->get_size());
            
            block = prev_block;
        }

        if (block->get_next()->get_state() == HeapBlock::State::Free) {

            HeapBlock* next_block = block->get_next();
            HeapBlock* next_next_block = next_block->get_next();

            /* Remove next_block from free list */
            size_t next_idx = get_bucket_index(next_block->get_size());
            remove_from_free_list(next_idx, next_block);

            /* Merge next_block into block */
            block->set_size(block->get_size() + next_block->get_size() + HeapBlock::size());
            next_next_block->set_prev_size(block->get_size());
        }

        block->get_next()->set_prev_state(HeapBlock::State::Free);
        block->set_state(HeapBlock::State::Free);

        /* Add the the recently freed block to the free list */
        size_t bucket = get_bucket_index(block->get_size());
        add_to_free_list(bucket, block);
    }

private:
    ALWAYS_INLINE size_t get_bucket_index(size_t size)
    {
        size_t log2 = 31 - __builtin_clz(size);
        if (log2 < first_bucket_log2)
            return 0;

        size_t bucket = log2 - first_bucket_log2;

        if (bucket > num_buckets)
            return num_buckets - 1;

        return bucket;
    }

    ALWAYS_INLINE void remove_from_free_list(size_t bucket, HeapBlock* block)
    {
        HeapBlock* prev_free = block->get_prev_free();
        HeapBlock* next_free = block->get_next_free();

        if (prev_free)
            prev_free->set_next_free(next_free);
        else
            m_buckets[bucket] = next_free;

        if (next_free)
            next_free->set_prev_free(prev_free);
    }

    ALWAYS_INLINE void add_to_free_list(size_t bucket, HeapBlock* block)
    {
        HeapBlock* first_node = m_buckets[bucket];

        if (first_node) {
            first_node->set_prev_free(block);
        }

        m_buckets[bucket] = block;
        block->set_next_free(first_node);
        block->set_prev_free(nullptr);
    }

    ALWAYS_INLINE constexpr bool should_split(size_t size, size_t block_size)
    {
        // for now we split everything as soon as a whole HeapBlock can fit there
        constexpr size_t threshold = sizeof(HeapBlock);
        return (block_size - size) >= threshold;
    }

    ALWAYS_INLINE constexpr bool is_heap_ptr(void* ptr)
    {
        FlatPtr fptr = reinterpret_cast<FlatPtr>(ptr);
        return fptr >= m_start && fptr <= m_end;
    }

private:
    FlatPtr m_start { 0 };
    FlatPtr m_end { 0 };
    HeapBlock* m_buckets[num_buckets] { nullptr };
};