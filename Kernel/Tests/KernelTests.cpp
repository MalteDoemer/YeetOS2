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

#include "Kernel/SerialDebug.hpp"
#include "Kernel/KernelTests.hpp"

namespace Kernel::Tests {

typedef bool (*TestFunc)();

/* These are all symbols defined by the linker script */
extern "C" void num_test_funcs();

extern "C" TestFunc test_funcs_start;
extern "C" TestFunc test_funcs_end;

extern "C" const char* test_names_start;
extern "C" const char* test_names_end;

void run_all_tests()
{
    TestResult result;

    /* num_tests must be volatile, because clang tries to optimize it away */
    volatile size_t num_tests = (volatile size_t)num_test_funcs;
    if (num_tests == 0) {
        Serial::println("[Kernel Tests]: Warning: no test available!");
        return;
    }

    TestFunc* test_func = &test_funcs_start;
    const char** test_name = &test_names_start;

    do {
        Serial::print("running test case ");
        Serial::print(*test_name);
        Serial::println(" ...");

        if ((*test_func)()) {
            result.num_tests_passed++;
            Serial::println("passed");
        } else {
            result.num_tests_failed++;
            Serial::println("failed");
        }

        Serial::println();
        result.num_tests_run++;

        test_func++;
        test_name++;
    } while (test_func < &test_funcs_end);

    Serial::print("total tests: ");
    Serial::println(result.num_tests_run);
    Serial::print("passed: ");
    Serial::println(result.num_tests_passed);
    Serial::print("failed: ");
    Serial::println(result.num_tests_failed);
    Serial::println();

    if (result.num_tests_failed == 0) {
        Serial::println("All tests passed successfully.");
    } else {
        Serial::println("Test execution failed.");
    }
}

}
