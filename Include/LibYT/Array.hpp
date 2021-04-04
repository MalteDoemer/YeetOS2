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
#include "Iterator.hpp"

namespace YT {

template<typename T, size_t Size>
requires requires
{
    requires Size > 0;
}
class Array {

public:
    using ValueType = T;
    using SizeType = size_t;
    using DifferenceType = ptrdiff_t;
    using Pointer = T*;
    using Reference = T&;
    using ConstPointer = const T*;
    using ConstReference = const T&;
    using Iterator = Pointer;
    using ConstIterator = ConstPointer;

public:
    constexpr SizeType count() const { return Size; }
    constexpr bool is_empty() const { return Size == 0; }

    constexpr Iterator begin() { return m_data; }
    constexpr ConstIterator begin() const { return m_data; }

    constexpr Iterator end() { return m_data + Size; }
    constexpr ConstIterator end() const { return m_data + Size; }

    constexpr ConstReference at(DifferenceType index) const { return m_data[index]; }
    constexpr Reference at(DifferenceType index) { return m_data[index]; }

    constexpr ConstReference front() const { return m_data[0]; }
    constexpr Reference front() { return m_data[0]; }

    constexpr ConstReference back() const { return m_data[Size - 1]; }
    constexpr Reference back() { return m_data[Size - 1]; }

    constexpr ConstPointer data() const { return m_data; }
    constexpr Pointer data() { return m_data; }

    constexpr ConstReference operator[](DifferenceType index) const { return m_data[index]; }
    constexpr Reference operator[](DifferenceType index) { return m_data[index]; }

public:
    T m_data[Size];
};

}

using YT::Array;
