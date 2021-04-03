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

#include "StdLibExtras.hpp"
#include "Types.hpp"

namespace YT {

template<class T>
concept ArithmeticType = IsArithmetic<T>::value;

template<class T>
concept ConstType = IsConst<T>::value;

template<class T>
concept EnumType = IsEnum<T>::value;

template<class T>
concept FloatingPointType = IsFloatingPoint<T>::value;
template<class T>

concept FundamentalType = IsFundamental<T>::value;
template<class T>

concept IntegralType = IsIntegral<T>::value;
template<class T>

concept NullPointerType = IsNullPointer<T>::value;
template<class T>

concept SigendType = IsSigned<T>::value;
template<class T>

concept UnionType = IsUnion<T>::value;

template<class T>
concept UnsigendType = IsUnsigned<T>::value;

template<class T, class U>
concept SameAs = IsSame<T, U>::value;

template<class From, class To>
concept ConvertibleTo = IsConvertible<From, To>::value;

template<class T>
concept EqualityCompareable = requires (T a, T b)
{
    { a == b } -> ConvertibleTo<bool>;
    { a != b } -> ConvertibleTo<bool>;
};


template<class T>
concept Compareable = requires (T a, T b)
{
    { a == b } -> ConvertibleTo<bool>;
    { a != b } -> ConvertibleTo<bool>;
    { a <= b } -> ConvertibleTo<bool>;
    { a >= b } -> ConvertibleTo<bool>;
    { a < b } -> ConvertibleTo<bool>;
    { a > b } -> ConvertibleTo<bool>;
};



}

using YT::ArithmeticType;
using YT::Compareable;
using YT::ConstType;
using YT::ConvertibleTo;
using YT::EnumType;
using YT::EqualityCompareable;
using YT::FloatingPointType;
using YT::FundamentalType;
using YT::IntegralType;
using YT::NullPointerType;
using YT::SameAs;
using YT::SigendType;
using YT::UnionType;
using YT::UnsigendType;