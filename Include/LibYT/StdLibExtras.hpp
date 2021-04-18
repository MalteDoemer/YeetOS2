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
#include "Verify.hpp"

namespace YT {

template<typename T> auto declval() -> T;

template<typename T, typename SizeType = decltype(sizeof(T)), SizeType N> constexpr SizeType array_size(T (&)[N])
{
    return N;
}

template<bool B, class T = void> struct EnableIf {
};

template<class T> struct EnableIf<true, T> {
    using Type = T;
};

template<class T> struct AddConst {
    using Type = const T;
};

template<class T> struct RemoveConst {
    using Type = T;
};

template<class T> struct RemoveConst<const T> {
    using Type = T;
};

template<class T> struct RemoveVolatile {
    using Type = T;
};
template<class T> struct RemoveVolatile<volatile T> {
    using Type = T;
};
template<class T> struct RemoveCV {
    using Type = typename RemoveVolatile<typename RemoveConst<T>::Type>::Type;
};

template<class T, T v> struct IntegralConstant {
    static constexpr T value = v;
    using ValueType = T;
    using Type = IntegralConstant;
    constexpr operator ValueType() const { return value; }
    constexpr ValueType operator()() const { return value; }
};

using FalseType = IntegralConstant<bool, false>;
using TrueType = IntegralConstant<bool, true>;
template<typename...> using VoidType = void;

template<class T> struct IsLvalueReference : FalseType {
};

template<class T> struct IsLvalueReference<T&> : TrueType {
};

template<class T> struct __IsPointerHelper : FalseType {
};

template<class T> struct __IsPointerHelper<T*> : TrueType {
};

template<class T> struct IsPointer : __IsPointerHelper<typename RemoveCV<T>::Type> {
};

template<class> struct IsFunction : FalseType {
};

template<class Ret, class... Args> struct IsFunction<Ret(Args...)> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...)> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args...) const> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...) const> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args...) volatile> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...) volatile> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args...) const volatile> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...) const volatile> : TrueType {
};

template<class Ret, class... Args> struct IsFunction<Ret(Args...)&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...)&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args...) const&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...) const&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args...) volatile&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...) volatile&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args...) const volatile&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...) const volatile&> : TrueType {
};

template<class Ret, class... Args> struct IsFunction<Ret(Args...) &&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...) &&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args...) const&&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...) const&&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args...) volatile&&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...) volatile&&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args...) const volatile&&> : TrueType {
};
template<class Ret, class... Args> struct IsFunction<Ret(Args..., ...) const volatile&&> : TrueType {
};

template<class T> struct IsRvalueReference : FalseType {
};
template<class T> struct IsRvalueReference<T&&> : TrueType {
};

template<class T> struct RemovePointer {
    using Type = T;
};
template<class T> struct RemovePointer<T*> {
    using Type = T;
};
template<class T> struct RemovePointer<T* const> {
    using Type = T;
};
template<class T> struct RemovePointer<T* volatile> {
    using Type = T;
};
template<class T> struct RemovePointer<T* const volatile> {
    using Type = T;
};

template<typename T, typename U> struct IsSame {
    static constexpr bool value = false;
};

template<typename T> struct IsSame<T, T> {
    static constexpr bool value = true;
};

template<bool condition, class TrueType, class FalseType> struct Conditional {
    using Type = TrueType;
};

template<class TrueType, class FalseType> struct Conditional<false, TrueType, FalseType> {
    using Type = FalseType;
};

template<typename T> struct IsNullPointer : IsSame<decltype(nullptr), typename RemoveCV<T>::Type> {
};

template<typename T> struct RemoveReference {
    using Type = T;
};
template<class T> struct RemoveReference<T&> {
    using Type = T;
};
template<class T> struct RemoveReference<T&&> {
    using Type = T;
};

template<class T> constexpr T&& forward(typename RemoveReference<T>::Type& param)
{
    return static_cast<T&&>(param);
}

template<class T> constexpr T&& forward(typename RemoveReference<T>::Type&& param) noexcept
{
    static_assert(!IsLvalueReference<T>::value, "Can't forward an rvalue as an lvalue.");
    return static_cast<T&&>(param);
}

template<typename T> struct MakeUnsigned {
    using Type = void;
};
template<> struct MakeUnsigned<signed char> {
    using Type = unsigned char;
};
template<> struct MakeUnsigned<short> {
    using Type = unsigned short;
};
template<> struct MakeUnsigned<int> {
    using Type = unsigned int;
};
template<> struct MakeUnsigned<long> {
    using Type = unsigned long;
};
template<> struct MakeUnsigned<long long> {
    using Type = unsigned long long;
};
template<> struct MakeUnsigned<unsigned char> {
    using Type = unsigned char;
};
template<> struct MakeUnsigned<unsigned short> {
    using Type = unsigned short;
};
template<> struct MakeUnsigned<unsigned int> {
    using Type = unsigned int;
};
template<> struct MakeUnsigned<unsigned long> {
    using Type = unsigned long;
};
template<> struct MakeUnsigned<unsigned long long> {
    using Type = unsigned long long;
};
template<> struct MakeUnsigned<char> {
    using Type = unsigned char;
};
template<> struct MakeUnsigned<char8_t> {
    using Type = char8_t;
};
template<> struct MakeUnsigned<char16_t> {
    using Type = char16_t;
};
template<> struct MakeUnsigned<char32_t> {
    using Type = char32_t;
};
template<> struct MakeUnsigned<bool> {
    using Type = bool;
};

template<typename T> struct MakeSigned {
};
template<> struct MakeSigned<signed char> {
    using Type = signed char;
};
template<> struct MakeSigned<short> {
    using Type = short;
};
template<> struct MakeSigned<int> {
    using Type = int;
};
template<> struct MakeSigned<long> {
    using Type = long;
};
template<> struct MakeSigned<long long> {
    using Type = long long;
};
template<> struct MakeSigned<unsigned char> {
    using Type = char;
};
template<> struct MakeSigned<unsigned short> {
    using Type = short;
};
template<> struct MakeSigned<unsigned int> {
    using Type = int;
};
template<> struct MakeSigned<unsigned long> {
    using Type = long;
};
template<> struct MakeSigned<unsigned long long> {
    using Type = long long;
};
template<> struct MakeSigned<char> {
    using Type = signed char;
};

template<class T> struct IsVoid : IsSame<void, typename RemoveCV<T>::Type> {
};

template<class T> struct IsConst : FalseType {
};

template<class T> struct IsConst<const T> : TrueType {
};

template<typename T, typename U = T> constexpr T exchange(T& slot, U&& value)
{
    T old_value = move(slot);
    slot = forward<U>(value);
    return old_value;
}

template<typename T> struct IsEnum : public IntegralConstant<bool, __is_enum(T)> {
};

template<typename T> struct IsUnion : public IntegralConstant<bool, __is_union(T)> {
};

template<typename T> struct IsClass : public IntegralConstant<bool, __is_class(T)> {
};

template<typename Base, typename Derived> struct IsBaseOf : public IntegralConstant<bool, __is_base_of(Base, Derived)> {
};

template<typename T> constexpr bool is_trivial()
{
    return __is_trivial(T);
}

template<typename T> constexpr bool is_trivially_copyable()
{
    return __is_trivially_copyable(T);
}

template<typename T> struct __IsIntegral : FalseType {
};
template<> struct __IsIntegral<bool> : TrueType {
};
template<> struct __IsIntegral<unsigned char> : TrueType {
};
template<> struct __IsIntegral<char8_t> : TrueType {
};
template<> struct __IsIntegral<char16_t> : TrueType {
};
template<> struct __IsIntegral<char32_t> : TrueType {
};
template<> struct __IsIntegral<unsigned short> : TrueType {
};
template<> struct __IsIntegral<unsigned int> : TrueType {
};
template<> struct __IsIntegral<unsigned long> : TrueType {
};
template<> struct __IsIntegral<unsigned long long> : TrueType {
};
template<typename T> using IsIntegral = __IsIntegral<typename MakeUnsigned<typename RemoveCV<T>::Type>::Type>;

template<typename T> struct __IsFloatingPoint : FalseType {
};
template<> struct __IsFloatingPoint<float> : TrueType {
};
template<> struct __IsFloatingPoint<double> : TrueType {
};
template<> struct __IsFloatingPoint<long double> : TrueType {
};
template<typename T> using IsFloatingPoint = __IsFloatingPoint<typename RemoveCV<T>::Type>;

template<typename ReferenceType, typename T>
using CopyConst = typename Conditional<IsConst<ReferenceType>::value, typename AddConst<T>::Type,
    typename RemoveConst<T>::Type>::Type;

template<typename... Ts> using Void = void;

template<typename... _Ignored> constexpr auto DependentFalse = false;

template<typename T> using IsSigned = IsSame<T, typename MakeSigned<T>::Type>;

template<typename T> using IsUnsigned = IsSame<T, typename MakeUnsigned<T>::Type>;

template<typename T> using IsArithmetic = IntegralConstant<bool, IsIntegral<T>::value || IsFloatingPoint<T>::value>;

template<typename T>
using IsFundamental = IntegralConstant<bool, IsArithmetic<T>::value || IsVoid<T>::value || IsNullPointer<T>::value>;

template<typename T, T... Ts> struct IntegerSequence {
    using Type = T;
    static constexpr unsigned size() noexcept { return sizeof...(Ts); };
};

template<unsigned... Indices> using IndexSequence = IntegerSequence<unsigned, Indices...>;

template<typename T, T N, T... Ts> auto make_integer_sequence_impl()
{
    if constexpr (N == 0)
        return IntegerSequence<T, Ts...> {};
    else
        return make_integer_sequence_impl<T, N - 1, N - 1, Ts...>();
}

template<typename T, T N> using MakeIntegerSequence = decltype(make_integer_sequence_impl<T, N>());

template<unsigned N> using MakeIndexSequence = MakeIntegerSequence<unsigned, N>;

template<typename T> struct IdentityType {
    using Type = T;
};

template<class T, bool = IsEnum<T>::value> struct __UnderlyingType {
    using Type = __underlying_type(T);
};

template<class T> struct __UnderlyingType<T, false> {
};

template<class T> struct UnderlyingType : __UnderlyingType<T> {
};

namespace Detail::IsConvertible {

template<typename To> static void convert(To);

template<typename From, typename To>
auto test(From&&, To &&) -> decltype(convert<To>(declval<From>()), declval<TrueType>());
auto test(...) -> FalseType;

}

template<typename From, typename To>
struct IsConvertible : decltype(Detail::IsConvertible::test(declval<From>(), declval<To>())) {
};

constexpr bool is_constant_evaluated() 
{
    return __builtin_is_constant_evaluated();
}

}

using YT::AddConst;
using YT::array_size;
using YT::Conditional;
using YT::declval;
using YT::DependentFalse;
using YT::exchange;
using YT::forward;
using YT::IdentityType;
using YT::IndexSequence;
using YT::IntegerSequence;
using YT::is_trivial;
using YT::is_trivially_copyable;
using YT::IsArithmetic;
using YT::IsBaseOf;
using YT::IsClass;
using YT::IsConst;
using YT::IsConvertible;
using YT::IsEnum;
using YT::IsFloatingPoint;
using YT::IsFundamental;
using YT::IsIntegral;
using YT::IsNullPointer;
using YT::IsSame;
using YT::IsSigned;
using YT::IsUnion;
using YT::IsUnsigned;
using YT::IsVoid;
using YT::MakeIndexSequence;
using YT::MakeIntegerSequence;
using YT::MakeSigned;
using YT::MakeUnsigned;
using YT::RemoveConst;
using YT::UnderlyingType;
using YT::Void;
