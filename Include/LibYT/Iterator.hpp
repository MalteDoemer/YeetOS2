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

template<class Iter>
struct IteratorTraits {
    using ValueType = Iter::ValueType;
    using SizeType = Iter::SizeType;
    using DifferenceType = Iter::DifferenceType;
    using PointerType = Iter::PointerType;
    using ReferenceType = Iter::ReferenceType;
    using ConstPointerType = Iter::ConstPointerType;
    using ConstReferenceType = Iter::ConstReferenceType;
};

template<class T>
struct IteratorTraits<T*> {
    using ValueType = T;
    using SizeType = size_t;
    using DifferenceType = ptrdiff_t;
    using PointerType = T*;
    using ReferenceType = T&;
    using ConstPointerType = const T*;
    using ConstReferenceType = const T&;
};

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

#if 0
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
#endif


template<class T>
concept ContainerType = requires (T a, typename T::DifferenceType n){

    typename T::ValueType;
    typename T::SizeType;
    typename T::DifferenceType;
    typename T::PointerType;
    typename T::ReferenceType;
    typename T::ConstPointerType;
    typename T::ConstReferenceType;
    typename T::IteratorType;

    { a.begin() } -> IteratorType;
    { a.end() } -> IteratorType;
    { a.count() } -> ConvertibleTo<typename T::SizeType>;
    { a.at(n) } -> ConvertibleTo<typename T::ValueType>;
};


template<ContainerType Container>
class Iterator {

public:
    using ValueType = Container::ValueType;
    using SizeType = Container::SizeType;
    using DifferenceType = Container::DifferenceType;
    using PointerType = Container::PointerType;
    using ReferenceType = Container::ReferenceType;
    using ConstPointerType = Container::ConstPointerType;
    using ConstReferenceType = Container::ConstReferenceType;

public:
    ALWAYS_INLINE constexpr Iterator(Container& container) : 
        m_container(container), m_index(0) {}

    ALWAYS_INLINE constexpr Iterator(Container& container, SizeType index) : 
        m_container(container), m_index(pos) {}

    ALWAYS_INLINE constexpr Iterator(const Iterator& other) = default;

    ALWAYS_INLINE constexpr Iterator& operator=(const Iterator& other){
        m_index = other.m_index;
        m_container = other.m_container;
        return *this;
    }

public:
    ALWAYS_INLINE constexpr SizeType index(){ return m_index; }

    ALWAYS_INLINE constexpr Iterator operator+(DifferenceType delta) const { return Iterator {m_container, m_index + delta }; }
    ALWAYS_INLINE constexpr Iterator operator-(DifferenceType delta) const { return Iterator {m_container, m_index - delta }; }

    ALWAYS_INLINE constexpr Iterator operator+=(DifferenceType delta) const { m_index += delta; return *this; }
    ALWAYS_INLINE constexpr Iterator operator-=(DifferenceType delta) const { m_index -= delta; return *this; }    

    ALWAYS_INLINE constexpr Iterator operator++() { ++m_index; return *this; }
    ALWAYS_INLINE constexpr Iterator operator++(int) { return Iterator { m_container, m_index++ }; }

    ALWAYS_INLINE constexpr Iterator operator--() { --m_index; return *this; }
    ALWAYS_INLINE constexpr Iterator operator--(int) { return Iterator { m_container, m_index-- }; }

    ALWAYS_INLINE constexpr bool operator==(Iterator other) const { return m_index == other.m_index; }
    ALWAYS_INLINE constexpr bool operator!=(Iterator other) const { return m_index != other.m_index; }
    ALWAYS_INLINE constexpr bool operator<=(Iterator other) const { return m_index <= other.m_index; }
    ALWAYS_INLINE constexpr bool operator>=(Iterator other) const { return m_index >= other.m_index; }
    ALWAYS_INLINE constexpr bool operator<(Iterator other) const { return m_index < other.m_index; }
    ALWAYS_INLINE constexpr bool operator>(Iterator other) const { return m_index > other.m_index; }

    ALWAYS_INLINE constexpr ReferenceType operator*() { return m_container.at(m_index); }
    ALWAYS_INLINE constexpr ConstReferenceType operator*() const { return m_container.at(m_index); }

    ALWAYS_INLINE constexpr PointerType operator->() { return &m_container.at(m_index); }
    ALWAYS_INLINE constexpr ConstPointerType operator->() const { return &m_container.at(m_index); }

private:
    Container& m_container;
    SizeType m_index;
};

}

using YT::IteratorTraits;
using YT::ContainerType;
using YT::IteratorType;
using YT::Iterator;