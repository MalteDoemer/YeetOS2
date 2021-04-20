/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
 * Copyright (C) 2021 Malte Dömer
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <Verify.hpp>
#include <UniquePtr.hpp>
#include <StdLibExtras.hpp>

namespace YT {

// template<typename T> class Function;

template<typename Out, typename... In> class Function {
public:
    Function() = default;

    template<typename T>
        requires(!IsPointer<T>::value)
        && IsFunction<T>::value&& IsRvalueReference<T&&>::value Function(T&& callable) :
        m_callable_wrapper(make<CallableWrapper<T>>(move(callable)))
    {
    }

    template<typename FunctionType>
    requires IsPointer<FunctionType>::value&& IsFunction<typename RemovePointer<FunctionType>::Type>::value Function(
        FunctionType f) :
        m_callable_wrapper(make<CallableWrapper<FunctionType>>(move(f)))
    {
    }

    Out operator()(In... in) const
    {
        VERIFY(m_callable_wrapper);
        return m_callable_wrapper->call(forward<In>(in)...);
    }

    explicit operator bool() const { return !!m_callable_wrapper; }

    template<typename T>
        requires(!IsPointer<T>::value)
        && IsFunction<T>::value&& IsRvalueReference<T&&>::value Function& operator=(T&& callable)
    {
        m_callable_wrapper = make<CallableWrapper<T>>(move(callable));
        return *this;
    }

    template<typename T>
    requires IsPointer<T>::value&& IsFunction<typename RemovePointer<T>::Type>::value Function& operator=(T f)
    {
        m_callable_wrapper = make<CallableWrapper<T>>(move(f));
        return *this;
    }

    Function& operator=(std::nullptr_t)
    {
        m_callable_wrapper = nullptr;
        return *this;
    }

private:
    class CallableWrapperBase {
    public:
        virtual ~CallableWrapperBase() = default;
        virtual Out call(In...) const = 0;
    };

    template<typename CallableType> class CallableWrapper final : public CallableWrapperBase {
    public:
        explicit CallableWrapper(CallableType&& callable) : m_callable(move(callable)) {}

        CallableWrapper(const CallableWrapper&) = delete;
        CallableWrapper& operator=(const CallableWrapper&) = delete;

        Out call(In... in) const final override
        {
            if constexpr (requires { m_callable(forward<In>(in)...); }) {
                return m_callable(forward<In>(in)...);
            } else if constexpr (requires { m_callable(); }) {
                return m_callable();
            } else if constexpr (IsVoid<Out>::value) {
                return;
            } else {
                return {};
            }
        }

    private:
        CallableType m_callable;
    };

    UniquePtr<CallableWrapperBase> m_callable_wrapper;
};

}

using YT::Function;
