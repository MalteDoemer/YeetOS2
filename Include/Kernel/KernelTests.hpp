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

#ifdef __KERNEL_TESTS__

#include "Platform.hpp"

#include "Kernel/Kernel.hpp"
#include "Kernel/SerialDebug.hpp"

#define TEST_CASE(func)                                                                                                \
    static bool func();                                                                                                \
    SECTION(".kernel_test_funcs")                                                                                      \
    bool (*__##func##_ptr)() = func;                                                                                   \
    SECTION(".kernel_test_names")                                                                                      \
    const char* __##func##_name = #func;                                                                               \
    static bool func()

#define EXPECT(x)                                                                                                      \
    do {                                                                                                               \
        if (!x) {                                                                                                      \
            Serial::print(__PRETTY_FUNCTION__);                                                                        \
            Serial::println(": expect failed: " #x);                                                                   \
            return false;                                                                                              \
        }                                                                                                              \
    } while (false)

#define EXPECT_EQU(a, b)                                                                                               \
    do {                                                                                                               \
        auto lhs = a;                                                                                                  \
        auto rhs = b;                                                                                                  \
        if (lhs != rhs) {                                                                                              \
            Serial::print(__PRETTY_FUNCTION__);                                                                        \
            Serial::println(": expect equal failed: " #a "was not equal to" #b);                                       \
            return false;                                                                                              \
        }                                                                                                              \
    } while (false)

#define EXPECT_NOT_EQU(a, b)                                                                                           \
    do {                                                                                                               \
        auto lhs = a;                                                                                                  \
        auto rhs = b;                                                                                                  \
        if (lhs == rhs) {                                                                                              \
            Serial::print(__PRETTY_FUNCTION__);                                                                        \
            Serial::println(": expect not equal failed: " #a "was equal to" #b);                                       \
            return false;                                                                                              \
        }                                                                                                              \
    } while (false)

#define EXPECT_ALL_EQU(...)                                                                                            \
    do {                                                                                                               \
        auto list = { __VA_ARGS__ };                                                                                   \
        for (size_t j = 0; j < list.count(); j++)                                                                      \
            for (size_t i = 0; i < list.count(); i++)                                                                  \
                if (list[i] != list[j]) {                                                                              \
                    Serial::print(__PRETTY_FUNCTION__);                                                                \
                    Serial::print(": expect all equal failed: i=");                                                    \
                    Serial::print(i);                                                                                  \
                    Serial::print(" j=");                                                                              \
                    Serial::println(j);                                                                                \
                    return false;                                                                                      \
                }                                                                                                      \
    } while (false)

namespace Kernel::Tests {

struct TestResult {
    i32 num_tests_run { 0 };
    i32 num_tests_passed { 0 };
    i32 num_tests_failed { 0 };
};

void run_all_tests();
}

#endif