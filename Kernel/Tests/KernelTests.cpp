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

extern "C" TestFunc test_funcs_start;
extern "C" TestFunc test_funcs_end;

extern "C" const char* test_names_start;
extern "C" const char* test_names_end;

void run_all_tests()
{
    TestResult result;

    if (&test_funcs_start == &test_funcs_end) {
        Serial::println("[Kernel Tests]: Warning: no test available!");
        return;
    }

    TestFunc* test_func = &test_funcs_start;
    const char** test_name = &test_names_start;

    do {
        Serial::print("Running test: ");
        Serial::println(*test_name);

        if ((*test_func)()) {
            result.num_tests_passed++;
        } else {
            result.num_tests_failed++;
        }
        result.num_tests_run++;

        test_func++;
        test_name++;
    } while (test_func < &test_funcs_end);

    Serial::println("\nFinished running tests.\n");
}

}
