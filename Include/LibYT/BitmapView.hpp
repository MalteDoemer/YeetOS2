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
#include "Concepts.hpp"
#include "Iterator.hpp"

template<IntegralType T = Uint32> class BitmapView {

public:
    using ValueType = bool;
    using ImplType = T;
    using SizeType = size_t;
    using DifferenceType = ptrdiff_t;

    using Pointer = bool;
    using Reference = bool;
    using ConstPointer = bool;
    using ConstReference = bool;

    using Iterator = SimpleIterator<const BitmapView>;
    using ConstIterator = SimpleIterator<const BitmapView>;

    constexpr static auto bits_per_entry = sizeof(ImplType) * 8;

public:
    constexpr BitmapView(ImplType* memory, SizeType count) : m_memory(memory), m_count(count) {}

    constexpr SizeType count() const { return m_count; }

    constexpr Iterator begin() { return Iterator { *this }; };
    constexpr ConstIterator begin() const { return ConstIterator { *this }; }

    constexpr Iterator end() { return Iterator { *this, m_count }; }
    constexpr ConstIterator end() const { return ConstIterator { *this, m_count }; }

    constexpr ValueType at(SizeType index) const { return get_bit(index); }

    constexpr ValueType get_bit(SizeType index) const
    {
        VERIFY(index < m_count);
        return (m_bits[index / bits_per_entry] & (1u << (index % bits_per_entry))) != 0;
    }

    constexpr ValueType set_bit(SizeType index)
    {
        VERIFY(index < m_count);
        return m_bits[index / bits_per_entry] |= (1u << (index % bits_per_entry));
    }

    constexpr ValueType clear_bit(SizeType index)
    {
        VERIFY(index < m_count);
        return m_bits[index / bits_per_entry] &= ~(1u << (index % bits_per_entry));
    }

private:
    ImplType* m_memory { nullptr };
    SizeType m_count { 0 };
};