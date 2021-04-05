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

template<class T>
class Span {

public:
    using ValueType = T;
    using SizeType = size_t;
    using DifferenceType = ptrdiff_t;
    using Reference = T&;
    using ConstReference = const T&;
    using Pointer = T*;
    using ConstPointer = const T*;
    using Iterator = T*;
    using ConstIterator = const T*;

public:
    constexpr Span() = default;

    constexpr Span(Pointer data, SizeType size) :
        m_data(data), m_size(size) {}

    template<SizeType size>
    constexpr Span(T (&data)[size]) :
        m_data(data), m_size(size) {}

    constexpr Span(const Span& other) :
        m_data(other.m_data), m_size(other.m_size) {}

    constexpr Span& operator=(const Span& other)
    {
        m_data = other.m_data;
        m_size = other.m_size;
    }

    constexpr operator Span<const T>() const
    {
        return { data(), count() };
    }

    constexpr SizeType count() const { return m_size; }
    constexpr bool is_null() const { return m_data == nullptr; }
    constexpr bool is_empty() const { return m_size == 0; }

    constexpr const T* data() const { return m_data; }
    constexpr T* data() { return m_data; }

    constexpr Iterator begin() { return data(); }
    constexpr ConstIterator begin() const { return data(); }

    constexpr Iterator end() { return data() + count(); }
    constexpr ConstIterator end() const { return data() + count(); }

    constexpr Reference at(DifferenceType index)
    {
        VERIFY(index < count());
        return m_data[index];
    }
    constexpr ConstReference at(DifferenceType index) const
    {
        VERIFY(index < count());
        return m_data[index];
    }

    constexpr Reference front() { return at(0); }
    constexpr ConstReference front() const { return at(0); }

    constexpr Reference back() { return at(count() - 1); }
    constexpr ConstReference back() const { return at(count() - 1); }

    constexpr Reference operator[](DifferenceType index) { return at(index); }
    constexpr ConstReference operator[](DifferenceType index) const { return at(index); }

    [[nodiscard]] constexpr Span subspan(SizeType start, SizeType length) const
    {
        VERIFY(start + length <= count());
        return Span { m_data + start, length };
    }

    [[nodiscard]] constexpr Span subspan(SizeType start) const
    {
        VERIFY(start <= count());
        return Span { m_data + start, count() - start };
    }

    [[nodiscard]] constexpr Span trim(SizeType end) const
    {
        return Span { m_data, min(end, count()) };        
    }

private:
    T* m_data;
    SizeType m_size;
};
