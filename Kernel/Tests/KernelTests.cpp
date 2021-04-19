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

struct Test {
    TestFunc func;
    const char* name;
};

static constexpr size_t test_max = 1024;

static size_t test_num;
static Test tests[test_max];

void add_test_case(TestFunc func, const char* name)
{
    if (test_num >= test_max)
        return;

    tests[test_num].func = func;
    tests[test_num].name = name;
    test_num++;
}

void run_all_tests()
{
    TestResult result;

    for (size_t i = 0; i < test_num; i++) {
        Serial::print("running test case ");
        Serial::print(tests[i].name);
        Serial::println(" ...");

        if (tests[i].func()) {
            result.num_tests_passed++;
            Serial::println("passed");
        } else {
            result.num_tests_failed++;
            Serial::println("failed");
        }

        Serial::println();
        result.num_tests_run++;
    }

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
