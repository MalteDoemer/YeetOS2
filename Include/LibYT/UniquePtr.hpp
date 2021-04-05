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
#include "StdLibExtras.hpp"

namespace YT {

template<class T>
class UniquePtr {

public:

    constexpr UniquePtr() :
        m_ptr(nullptr) {}

    constexpr explicit UniquePtr(T* ptr) :
        m_ptr(ptr) {}

    constexpr UniquePtr(UniquePtr&& ptr) :
        m_ptr(ptr.release_ptr()) {}

    template<class U>
    constexpr UniquePtr(UniquePtr<U>&& ptr) :
        m_ptr(ptr.release_ptr()) {}

    constexpr UniquePtr& operator=(UniquePtr&& other)
    {
        UniquePtr ptr(move(other));
        swap(ptr);
        return *this;
    }

    template<class U>
    constexpr UniquePtr& operator=(UniquePtr<U>&& other)
    {
        UniquePtr ptr(move(other));
        swap(ptr);
        return *this;
    }

    constexpr UniquePtr& operator=(T* ptr)
    {
        if (m_ptr != ptr)
            delete m_ptr;
        m_ptr = ptr;
        return *this;
    }

    constexpr UniquePtr& operator=(std::nullptr_t)
    {
        clear();
        return *this;
    }

    ~UniquePtr() { clear(); }

public:
    UniquePtr(const UniquePtr&) = delete;

    template<class U>
    UniquePtr(const UniquePtr<U>&) = delete;

    UniquePtr& operator=(const UniquePtr&) = delete;

    template<class U>
    UniquePtr& operator=(const UniquePtr<U>&) = delete;

public:
    constexpr void swap(UniquePtr& other)
    {
        YT::swap(m_ptr, other.m_ptr);
    }

    template<class U>
    constexpr void swap(UniquePtr<U>& other)
    {
        YT::swap(m_ptr, other.m_ptr);
    }

    constexpr void clear()
    {
        delete m_ptr;
        m_ptr = nullptr;
    }

    [[nodiscard]] constexpr T* release_ptr()
    {
        T* ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }

public:
    constexpr T* ptr() { return m_ptr; }
    constexpr const T* ptr() const { return m_ptr; }

    constexpr T* operator->() { return m_ptr; }
    constexpr const T* operator->() const { return m_ptr; }

    constexpr T& operator*() { return *m_ptr; }
    constexpr const T& operator*() const { return *m_ptr; }

    constexpr operator T*() { return m_ptr; }
    constexpr operator const T*() const { return m_ptr; }

    constexpr operator bool() const { return !!m_ptr; }
    constexpr bool operator!() const { return !m_ptr; }

private:
    T* m_ptr;
};

template<class T>
void swap(UniquePtr<T> ptr1, UniquePtr<T> ptr2){
    ptr1.swap(ptr2);
}


template<class T, class U>
void swap(UniquePtr<T> ptr1, UniquePtr<U> ptr2){
    ptr1.swap(ptr2);
}


template<class T, class... Args>
UniquePtr<T> make_unique(Args&&... args)
{
    return UniquePtr<T>(new T(forward<Args>(args)...));
}

}

using YT::UniquePtr;
using YT::make_unique;
using YT::swap;
