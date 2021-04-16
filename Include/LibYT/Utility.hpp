/*
 * Copyright (c) 2018-2021, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2021, Malte Dömer
 * All rights reserved.
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
#include "StdLibExtras.hpp"

namespace std {

// NOTE: This is in the "std" namespace since some compiler features rely on it.

template<typename T> constexpr T&& move(T& arg)
{
    return static_cast<T&&>(arg);
}

}

namespace YT {

using std::move;

constexpr Uint32 round_up_to_next_power_of_two(Uint32 value)
{
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value++;
    return value;
}

template<typename T> constexpr T min(const T& a, const T& b)
{
    return b < a ? b : a;
}

template<typename T> constexpr T max(const T& a, const T& b)
{
    return a < b ? b : a;
}

template<typename T> constexpr T clamp(const T& value, const T& min, const T& max)
{
    VERIFY(max >= min);
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

template<typename T, typename U> constexpr T ceil_div(T a, U b)
{
    static_assert(sizeof(T) == sizeof(U));
    T result = a / b;
    if ((a % b) != 0)
        ++result;
    return result;
}

template<typename T, typename U> constexpr void swap(T& a, U& b)
{
    U tmp = move((U&)a);
    a = (T &&) move(b);
    b = move(tmp);
}

template<typename T> constexpr void copy(T* dest, const T* src, size_t count)
{
    if constexpr (is_trivial<T>()) {
        __builtin_memcpy(dest, src, count * sizeof(T));
    } else {
        while (count--) { new (dest++) T(*src++); }
    }
}

template<typename T> constexpr void move(T* dest, const T* src, size_t count)
{
    if constexpr (is_trivial<T>()) {
        __builtin_memmove(dest, src, count * sizeof(T));
    } else {
        if (dest > src) {
            dest = dest + count - 1;
            src = src + count - 1;

            while (count--) { new (dest--) T(move(*src--)); }
        } else {

            while (count--) { new (dest++) T(move(*src++)); }
        }
    }
}

template<typename T> constexpr void assign(T* dest, const T& value, size_t count)
{
    while (count--) { *dest++ = value; }
}

template<typename T> constexpr bool compare(T* a, T* b, size_t count)
{
    if constexpr (is_trivial<T>()) {
        return !__builtin_memcmp(a, b, count * sizeof(T));
    } else {
        for (size_t i = 0; i < count; i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }
}

}

using YT::assign;
using YT::ceil_div;
using YT::clamp;
using YT::compare;
using YT::copy;
using YT::max;
using YT::min;
using YT::move;
using YT::round_up_to_next_power_of_two;
using YT::swap;