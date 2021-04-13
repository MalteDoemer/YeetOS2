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

#include "Platform.hpp"
#include "Concepts.hpp"

namespace YT {

template<class Iter> struct IteratorTraits {
    using ValueType = typename Iter::ValueType;
    using SizeType = typename Iter::SizeType;
    using DifferenceType = typename Iter::DifferenceType;

    using Pointer = typename Iter::Pointer;
    using Reference = typename Iter::Reference;

    using ConstPointer = typename Iter::ConstPointer;
    using ConstReference = typename Iter::ConstReference;
};

template<class T> struct IteratorTraits<T*> {
    using ValueType = T;
    using SizeType = size_t;
    using DifferenceType = ptrdiff_t;
    using Pointer = T*;
    using Reference = T&;
    using ConstPointer = const T*;
    using ConstReference = const T&;
};

#if 0
template<class Iter>
concept IteratorType = requires(Iter i1, Iter i2)
{
    typename IteratorTraits<Iter>::ValueType;
    typename IteratorTraits<Iter>::SizeType;
    typename IteratorTraits<Iter>::DifferenceType;
    typename IteratorTraits<Iter>::PointerType;
    typename IteratorTraits<Iter>::ReferenceType;
    typename IteratorTraits<Iter>::ConstPointerType;
    typename IteratorTraits<Iter>::ConstReferenceType;


    { i1 == i2 } -> ConvertibleTo<bool>;
    { i1 != i2 } -> ConvertibleTo<bool>;

    { *i1 } -> ConvertibleTo<typename IteratorTraits<Iter>::ValueType>;

    { ++i1 } -> ConvertibleTo<Iter>;
    { i1++ } -> ConvertibleTo<Iter>;
    { --i1 } -> ConvertibleTo<Iter>;
    { i1-- } -> ConvertibleTo<Iter>;
};

template<class Iter>
concept AdvancedIterator = IteratorType<Iter> && requires(Iter i1, Iter i2, typename IteratorTraits<Iter>::DifferenceType n)
{     
    { i1 < i2 } -> ConvertibleTo<bool>;
    { i1 > i2 } -> ConvertibleTo<bool>;

    { i1 <= i2 } -> ConvertibleTo<bool>;
    { i1 >= i2 } -> ConvertibleTo<bool>;

    { i1 + n } -> ConvertibleTo<Iter>;
    { i1 - n } -> ConvertibleTo<Iter>;

    { i1 += n } -> ConvertibleTo<Iter>;
    { i1 -= n } -> ConvertibleTo<Iter>;

    { i1 - i2 } -> ConvertibleTo<typename IteratorTraits<Iter>::DifferenceType>;

    { i1[n] } -> ConvertibleTo<typename IteratorTraits<Iter>::ValueType>;

};


template<class T>
concept ContainerType = requires (T a, typename T::DifferenceType n){

    typename T::ValueType;
    typename T::SizeType;
    typename T::DifferenceType;
    typename T::PointerType;
    typename T::ReferenceType;
    typename T::ConstPointerType;
    typename T::ConstReferenceType;

    { a.begin() } -> IteratorType;
    { a.end() } -> IteratorType;
    { a.count() } -> ConvertibleTo<typename T::SizeType>;
    { a.at(n) } -> ConvertibleTo<typename T::ValueType>;
};

#endif

template<class Container> class SimpleIterator {

public:
    using ValueType = typename Container::ValueType;
    using SizeType = typename Container::SizeType;
    using DifferenceType = typename Container::DifferenceType;
    using Pointer = typename Container::Pointer;
    using Reference = typename Container::Reference;
    using ConstPointer = typename Container::ConstPointer;
    using ConstReference = typename Container::ConstReference;

public:
    constexpr SimpleIterator(Container& container) : m_container(container), m_index(0) {}

    constexpr SimpleIterator(Container& container, SizeType index) : m_container(container), m_index(index) {}

    constexpr SimpleIterator(const SimpleIterator& other) = default;

    constexpr SimpleIterator& operator=(const SimpleIterator& other)
    {
        m_index = other.m_index;
        m_container = other.m_container;
        return *this;
    }

public:
    constexpr SizeType index() { return m_index; }

    constexpr SimpleIterator operator+(DifferenceType delta) const
    {
        return SimpleIterator { m_container, m_index + delta };
    }
    constexpr SimpleIterator operator-(DifferenceType delta) const
    {
        return SimpleIterator { m_container, m_index - delta };
    }

    constexpr SimpleIterator operator+=(DifferenceType delta) const
    {
        m_index += delta;
        return *this;
    }
    constexpr SimpleIterator operator-=(DifferenceType delta) const
    {
        m_index -= delta;
        return *this;
    }

    constexpr SimpleIterator operator++()
    {
        ++m_index;
        return *this;
    }
    constexpr SimpleIterator operator++(int) { return SimpleIterator { m_container, m_index++ }; }

    constexpr SimpleIterator operator--()
    {
        --m_index;
        return *this;
    }
    constexpr SimpleIterator operator--(int) { return SimpleIterator { m_container, m_index-- }; }

    constexpr bool operator==(SimpleIterator other) const { return m_index == other.m_index; }
    constexpr bool operator!=(SimpleIterator other) const { return m_index != other.m_index; }
    constexpr bool operator<=(SimpleIterator other) const { return m_index <= other.m_index; }
    constexpr bool operator>=(SimpleIterator other) const { return m_index >= other.m_index; }
    constexpr bool operator<(SimpleIterator other) const { return m_index < other.m_index; }
    constexpr bool operator>(SimpleIterator other) const { return m_index > other.m_index; }

    constexpr Reference operator*() { return m_container.at(m_index); }
    constexpr ConstReference operator*() const { return m_container.at(m_index); }

    constexpr Reference operator[](DifferenceType index) { return m_container.at(index); }
    constexpr ConstReference operator[](DifferenceType index) const { return m_container.at(index); }

    constexpr Pointer operator->() { return &m_container.at(m_index); }
    constexpr ConstPointer operator->() const { return &m_container.at(m_index); }

private:
    Container& m_container;
    SizeType m_index;
};

}

using YT::IteratorTraits;
using YT::SimpleIterator;