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
#include "Concepts.hpp"

template<class T, IntegralType SizeType = size_t>
class Span {
private:
    T* m_data { nullptr };
    SizeType m_size { 0 };

public:
    using Span = Span<T, SizeType>;

    ALWAYS_INLINE constexpr Span() = default;

    ALWAYS_INLINE constexpr Span(T* data, SizeType size) :
        m_data(data), m_size(size) {}

    template<SizeType size>
    ALWAYS_INLINE constexpr Span(T (&data)[size]) :
        m_data(data), m_size(size) {}

    ALWAYS_INLINE constexpr Span(const Span& other) :
        m_data(other.m_data), m_size(other.m_size) {}

    ALWAYS_INLINE constexpr const T* data() const { return m_data; }
    ALWAYS_INLINE constexpr T* data() { return m_data; }

    ALWAYS_INLINE constexpr SizeType size() const { return m_size; }
    ALWAYS_INLINE constexpr bool is_null() const { return m_data == nullptr; }
    ALWAYS_INLINE constexpr bool is_empty() const { return m_size == 0; }

    ~Span();
};
