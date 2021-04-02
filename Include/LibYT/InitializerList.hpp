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

/* We need to use std here because the compiler needs it */
namespace std {

template <class T>
class initializer_list {
public:
    using ValueType = T;
    using Refrence = const T&;
    using ConstRefrence = const T&;
    using SizeType = __SIZE_TYPE__;
    using Iterator = const T*;
    using ConstIterator = const T*;

private:
    Iterator m_iter;
    SizeType m_size;

    constexpr initializer_list(ConstIterator __iter, SizeType __size)
        : m_iter(__iter), m_size(__size) { }

public:
    constexpr initializer_list()
        : m_iter(0), m_size(0) { }

    constexpr SizeType size() const { return m_size; }

    constexpr ConstIterator begin() { return m_iter; }

    constexpr ConstIterator end() { return begin() + size(); }
};
}

namespace YT {

template <class T>
constexpr const T* begin(std::initializer_list<T> __list)
{
    return __list.begin();
}

template <class T>
constexpr const T* end(std::initializer_list<T> __list)
{
    return __list.end();
}

}
