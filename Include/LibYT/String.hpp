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

template<class T, size_t size = sizeof(T) - 1> struct PaddingHelper {
    Uint8 padding[size];
};

template<class T> struct PaddingHelper<T, 1> {
};

}

template<class T>
concept CharType = IsStandardLayout<T>::value && (!IsArray<T>::value) && IsTrivial<T>::value;

template<CharType Char>
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
    /* like strlen just with our custom character type */
    static inline constexpr SizeType cstring_count(ConstPointer cstring)
    {
        ConstPointer start = cstring;
        while (*cstring != Char()) {
            cstring++;
        }
        return cstring - start;
    }

public:
    /* The (theoretically) maximum size of a String */
    inline constexpr static SizeType max_size() { return NumericLimits<SizeType>::max(); }

private:
    Pointer m_data;
    SizeType m_count;

    union {
        Char m_inline_buffer[num_elems];
        SizeType m_capcaity;
    };

private:
    /* Helper function that sets the count and writes the null terminator */
    inline constexpr void set_count(SizeType count)
    {
        m_count = count;
        m_data[count] = Char();
    }

    inline constexpr bool is_small() const { return m_data == m_inline_buffer; }

    inline constexpr void grow_if_needed()
    {
        if (count() == capacity()) {
            reserve(count() * 2);
        }
    }

    inline constexpr void init_capacity(SizeType cap)
    {
        if (cap > inline_capacity) {
            m_data = new Char[cap + 1];
            m_capcaity = cap;
        } else {
            m_data = m_inline_buffer;
        }
    }

public:
    inline constexpr BasicString()
    {
        m_data = m_inline_buffer;
        set_count(0);
    }

    inline constexpr BasicString(ConstPointer cstring) { assign(cstring); }

    inline constexpr BasicString(ConstPointer buffer, SizeType size) { assign(buffer, size); }

    inline constexpr BasicString(const BasicString& other) { assign(other); }

    inline constexpr BasicString(BasicString&& other) { assign(move(other)); }

    inline constexpr BasicString(std::initializer_list<Char> list) { assign(list); }

    inline constexpr BasicString& operator=(const BasicString& other)
    {
        if (&other != this) {
            assign(other);
        }
        return *this;
    }

    inline constexpr BasicString& operator=(BasicString&& other)
    {
        if (&other != this) {
            assign(other);
        }
        return *this;
    }

    inline constexpr BasicString& assign(ConstPointer cstring)
    {
        SizeType count = cstring_count(cstring);
        init_capacity(count);
        copy(data(), cstring, count);
        set_count(count);
        return *this;
    }

    inline constexpr BasicString& assign(ConstPointer buffer, SizeType count)
    {
        init_capacity(count);
        copy(data(), buffer, count);
        set_count(count);
        return *this;
    }

    inline constexpr BasicString& assign(const BasicString& other)
    {
        init_capacity(other.count());
        copy(m_data, other.data(), other.count());
        set_count(other.count());
        return *this;
    }

    inline constexpr BasicString& assign(BasicString&& other)
    {
        if (other.is_small()) {
            m_data = m_inline_buffer;
            copy(m_data, other.data(), other.count());
            set_count(other.count());
        } else {
            m_data = other.data();
            m_count = other.count();
            m_capcaity = other.capacity();
        }

        // make other an empty string
        other.m_data = other.m_inline_buffer;
        other.set_count(0);
        return *this;
    }

    inline constexpr BasicString& assign(std::initializer_list<Char> list)
    {
        assign(list.begin(), list.count());
        return *this;
    }

    ~BasicString()
    {
        if (!is_small())
            delete[] m_data;
    }

    inline constexpr SizeType count() const { return m_count; }
    inline constexpr bool is_empty() const { return count() == 0; }
    inline constexpr SizeType capacity() { return is_small() ? inline_capacity : m_capcaity; }

    inline constexpr Pointer data() { return m_data; }
    inline constexpr ConstPointer data() const { return m_data; }
    inline constexpr ConstPointer cstr() const { return m_data; }

    inline constexpr Reference at(SizeType index)
    {
        VERIFY(index < count());
        return data()[index];
    }

    inline constexpr ConstReference at(SizeType index) const
    {
        VERIFY(index < count());
        return data()[index];
    }

    inline constexpr Reference operator[](SizeType index) { return at(index); }
    inline constexpr ConstReference operator[](SizeType index) const { return at(index); }

    inline constexpr Reference front() { VERIFY(!is_empty()); return at(0); }
    inline constexpr ConstReference front() const { VERIFY(!is_empty()); return at(0); }

    inline constexpr Reference back() { VERIFY(!is_empty()); return at(count() - 1); }
    inline constexpr ConstReference back() const { VERIFY(!is_empty()); return at(count() - 1); }

    inline constexpr Iterator begin() { return Iterator(data()); }
    inline constexpr ConstIterator begin() const { return Iterator(data()); }

    inline constexpr Iterator end() { return Iterator(data() + count()); }
    inline constexpr ConstIterator end() const { return Iterator(data() + count()); }

    inline constexpr bool operator==(const BasicString& other) const
    {
        if (count() != other.count())
            return false;

        return equals(data(), other.data(), count());
    }

    inline constexpr bool operator==(ConstPointer cstring)
    {
        if (count() != cstring_count(cstring))
            return false;

        return equals(data(), cstring, count());
    }

    inline constexpr bool operator!=(const BasicString& other) const
    {
        if (count() != other.count())
            return true;

        return !equals(data(), other.data(), count());
    }

    inline constexpr bool operator!=(ConstPointer cstring)
    {
        if (count() != cstring_count(cstring))
            return true;

        return !equals(data(), cstring, count());
    }

    inline constexpr void reserve(SizeType new_cap)
    {
        if (new_cap <= capacity())
            return;

        Char* new_data = new Char[new_cap + 1];
        copy(new_data, m_data, count() + 1);

        if (!is_small())
            delete[] m_data;

        m_capcaity = new_cap;
        m_data = new_data;
    }

    inline constexpr void shrink_to_fit()
    {
        if (count() == capacity() || is_small())
            return;

        Char* new_data = nullptr;

        if (count() <= inline_capacity) {
            new_data = m_inline_buffer;
        } else {
            new_data = new Char[count() + 1];
            m_capcaity = count();
        }

        copy(new_data, m_data, count() + 1);
        delete[] m_data;
        m_data = new_data;
    }

    inline constexpr void clear()
    {
        if (!is_small())
            delete[] m_data;

        m_data = m_inline_buffer;
        set_count(0);
    }

    inline constexpr void resize(SizeType new_count)
    {
        resize(new_count, Char());
    }

    inline constexpr void resize(SizeType new_count, Char fill)
    {
        if (new_count > count()){
            reserve(new_count);
            YT::assign(data() + count(), fill, new_count - count());
            set_count(new_count);
        } else if (new_count < count()) {
            set_count(new_count);
            shrink_to_fit();
        }
    }

    inline constexpr void append(Char value)
    {
        grow_if_needed();
        data()[count()] = value;
        set_count(count() + 1);
    }

    inline constexpr void append(Char value, SizeType cnt)
    {
        reserve(count() + cnt);
        YT::assign(data() + count(), value, cnt);
        set_count(count() + cnt);
    }

    inline constexpr void append(ConstPointer other)
    {
        SizeType num = cstring_count(other);
        reserve(count() + num);
        copy(data() + count(), other, num);
        set_count(count() + num);
    }

    inline constexpr void append(ConstPointer other, SizeType num)
    {
        reserve(count() + num);
        copy(data() + count(), other, num);
        set_count(count() + num);
    }

    inline constexpr void append(const BasicString& other)
    {
        reserve(count() + other.count());
        move(data() + count(), other.data(), other.count());
        set_count(count() + other.count());
    }

    inline constexpr void append(const BasicString& other, SizeType num)
    {
        VERIFY(num <= other.count());
        reserve(count() + num);
        move(data() + count(), other.data(), num);
        set_count(count() + num);
    }

    inline constexpr void insert(SizeType index, SizeType cnt, Char c)
    {
        VERIFY(index < count());
        reserve(count() + cnt);
        move(data() + index + cnt, data() + index, count() - index);
        assign(data() + index, c, cnt);
        set_count(count() + cnt);
    }

    inline constexpr void insert(SizeType index, ConstPointer cstring)
    {
        VERIFY(index < count());
        SizeType cnt = cstring_count(cstring);
        reserve(count() + cnt);
        move(data() + index + cnt, data() + index, count() - index);
        copy(data() + index, cstring, cnt);
        set_count(count() + cnt);
    }

    inline constexpr void insert(SizeType index, const BasicString& str)
    {
        VERIFY(index < count());
        SizeType cnt = str.count();
        reserve(count() + cnt);
        move(data() + index + cnt, data() + index, count() - index);
        copy(data() + index, str.data(), cnt);
        set_count(count() + cnt);
    }

    inline constexpr void insert(SizeType index, SizeType cnt, ConstPointer cstring)
    {
        VERIFY(index < count());
        reserve(count() + cnt);
        move(data() + index + cnt, data() + index, count() - index);
        copy(data() + index, cstring, cnt);
        set_count(count() + cnt);
    }

    inline constexpr void insert(SizeType index, SizeType cnt, const BasicString& str)
    {
        VERIFY(index < count());
        reserve(count() + cnt);
        move(data() + index + cnt, data() + index, count() - index);
        copy(data() + index, str.data(), cnt);
        set_count(count() + cnt);
    }

    inline constexpr void erase(SizeType index, SizeType cnt)
    {
        VERIFY(index < count());
        SizeType to_remove = min(cnt, count() - index);
        move(data() + index, data() + index + to_remove, count() - index - to_remove);
        set_count(count() - to_remove);
    }

    inline constexpr void erase(ConstIterator first, ConstIterator last)
    {
        VERIFY(first >= begin());
        VERIFY(last <= end());
        SizeType to_remove = last - first;
        SizeType index = first - begin();
        move(first, last, count() - index - to_remove);
    }

};

template<CharType Char> 
inline constexpr bool operator==(const Char* lhs, const BasicString<Char> rhs)
{
    return rhs == lhs;
}

template<CharType Char> 
inline constexpr bool operator!=(const Char* lhs, const BasicString<Char> rhs)
{
    return rhs != lhs;
}


using String = BasicString<char>;
using WString = BasicString<wchar_t>;

}

using YT::BasicString;
using YT::String;
using YT::WString;