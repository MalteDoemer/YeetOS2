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

template<size_t num_bits, IntegralType T = Uint32>
class InlineBitmap {

public:
    using ValueType = bool;
    using ImplType = T;
    using SizeType = size_t;
    using DifferenceType = ptrdiff_t;
    using Iterator = const SimpleIterator<InlineBitmap>;
    using ConstIterator = const SimpleIterator<InlineBitmap>;

public:
    constexpr InlineBitmap() = default;

    constexpr SizeType count() const { return num_bits; }

    constexpr Iterator begin() { return Iterator { *this }; };
    constexpr ConstIterator begin() const { return ConstIterator { *this }; }

    constexpr Iterator end() { return Iterator { *this, num_bits }; }
    constexpr ConstIterator end() const { return ConstIterator { *this, num_bits }; }

    constexpr ValueType get_bit(SizeType index) const
    {
        VERIFY(index < num_bits);
        return (m_bits[index / sizeof(ImplType)] & (1u << (index % sizeof(ImplType)))) != 0;
    }

    constexpr ValueType set_bit(SizeType index)
    {
        VERIFY(index < num_bits);
        return m_bits[index / sizeof(ImplType)] |= (1u << (index % sizeof(ImplType)));
    }

    constexpr ValueType clear_bit(SizeType index)
    {
        VERIFY(index < num_bits);
        return m_bits[index / sizeof(ImplType)] &= ~(1u << (index % sizeof(ImplType)));
    }

private:
    ImplType m_bits[num_bits / sizeof(ImplType)] { 0 };
};