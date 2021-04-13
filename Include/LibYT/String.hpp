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

#include "New.hpp"
#include "Types.hpp"
#include "Concepts.hpp"
#include "Platform.hpp"
#include "Utility.hpp"
#include "StdLibExtras.hpp"
#include "NumericLimits.hpp"
#include "InitializerList.hpp"

namespace YT {

namespace Detail {

template<class T, size_t size = sizeof(T) - 1>
struct PaddingHelper {
    Uint8 padding[size];
};

template<class T>
struct PaddingHelper<T, 1> {
};

}

template<class Char>
class BasicString {

public:
    using ValueType = Char;
    using SizeType = size_t;
    using DifferenceType = ptrdiff_t;

    using Pointer = Char*;
    using Reference = Char&;
    using ConstPointer = const Char*;
    using ConstReference = const Char&;

    using Iterator = Pointer;
    using ConstIterator = ConstPointer;

private:
    static constexpr SizeType num_bytes = 32 - 2 * sizeof(void*);
    static constexpr SizeType num_elems = num_bytes / sizeof(Char);
    static constexpr SizeType inline_capacity = num_elems - 1;

    static_assert(num_elems > 0, "Character Type is to large!");

private:
    Pointer m_data;
    SizeType m_count;

    union {
        Char m_inline_buffer[num_elems];
        SizeType m_capcaity;
    };

    /* Helper function that sets the count and writes the null terminator */
    ALWAYS_INLINE constexpr void set_count(SizeType count)
    {
        m_count = count;
        m_data[count] = Char();
    }

    ALWAYS_INLINE constexpr bool is_small() const { return m_data == m_inline_buffer; }

    ALWAYS_INLINE constexpr void grow_if_needed()
    {
        if (count() == capacity()) {
            reserve(count() * 2);
        }
    }

public:
    ALWAYS_INLINE constexpr BasicString()
    {
        m_data = m_inline_buffer;
        set_count(0);
    }

    ALWAYS_INLINE constexpr explicit BasicString(ConstPointer cstring)
    {
        
    }

    ALWAYS_INLINE constexpr BasicString(ConstPointer buffer, SizeType num)
    {
    }

    ALWAYS_INLINE constexpr static SizeType max_size() { return NumericLimits<SizeType>::max(); }

    ALWAYS_INLINE constexpr SizeType count() const { return m_count; }
    ALWAYS_INLINE constexpr bool is_empty() const { return count() == 0; }
    ALWAYS_INLINE constexpr SizeType capacity() { return is_small() ? inline_capacity : m_capcaity; }

    ALWAYS_INLINE constexpr Pointer data() { return m_data; }
    ALWAYS_INLINE constexpr ConstPointer data() const { return m_data; }
    ALWAYS_INLINE constexpr ConstPointer cstr() const { return m_data; }

    ALWAYS_INLINE constexpr Reference at(SizeType pos) { return data()[pos]; }
    ALWAYS_INLINE constexpr ConstReference at(SizeType pos) const { return data()[pos]; }

    ALWAYS_INLINE constexpr Reference operator[](SizeType pos) { return at(pos); }
    ALWAYS_INLINE constexpr ConstReference operator[](SizeType pos) const { return at(pos); }

    inline constexpr void reserve(SizeType new_cap)
    {
        if (new_cap <= capacity())
            return;

        Char* new_data = new Char[new_cap];
        copy(new_data, m_data, count());
        new_data[count()] = Char();

        if (!is_small())
            delete[] m_data;

        m_capcaity = new_cap;
        m_data = new_data;
    }

    inline constexpr void shrink_to_fit()
    {
        if (count() == capacity() || is_small())
            return;

        Char* new_data = count() <= inline_capacity ? m_inline_buffer : new Char[count() + 1];
        copy(new_data, m_data, count());
        new_data[count()] = Char();
        delete[] m_data;

        m_capcaity = count();
        m_data = new_data;
    }

    inline constexpr void append(Char value)
    {
        grow_if_needed();
        at(count()) = value;
        set_count(count() + 1);
    }

    inline constexpr void append(const BasicString& other) {}

    inline constexpr void prepend(Char value) {}

    inline constexpr void prepend(const BasicString& other) {}
};

}

using YT::BasicString;