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

#include "Verify.hpp"
#include "UniquePtr.hpp"
#include "StdLibExtras.hpp"

namespace YT {

template<typename> class Function;

template<typename Ret, typename... Args> class Function<Ret(Args...)> {
public:

    Function() = default;

    template <typename CallableType>
    Function(CallableType callable) : 
        m_callable_wrapper(make_unique<CallableWrapper<CallableType>>(move(callable))) {}

    
    Ret operator()(Args... args) const 
    {
        VERIFY(m_callable_wrapper);
        return m_callable_wrapper->invoke(forward<Args>(args)...);
    }

    explicit operator bool() const { return !!m_callable_wrapper; }
    bool operator!() const { return !m_callable_wrapper; }

    template<typename CallableType>
    Function& operator=(CallableType callable) 
    {
        m_callable_wrapper = make_unique<CallableWrapper<CallableType>>(move(callable));
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
        virtual Ret invoke(Args...) const = 0;
    };

    template<typename T>
    class CallableWrapper final : public CallableWrapperBase {

    public:
        explicit CallableWrapper(T&& callable) : 
            m_callable(move(callable)) {}

        ~CallableWrapper() final override = default;

        CallableWrapper(const CallableWrapper&) = delete;
        CallableWrapper& operator=(const CallableWrapper&) = delete;

        Ret invoke(Args... args) const final override
        {
            if constexpr (requires { m_callable(forward<Args>(args)...); }){
                return m_callable(forward<Args>(args)...);
            } else if constexpr (requires { m_callable(); }){
                return m_callable();
            } else {
                VERIFY_NOT_REACHED();
            }
        }

    private:
        T m_callable;

    };


    UniquePtr<CallableWrapperBase> m_callable_wrapper;
};

}

using YT::Function;