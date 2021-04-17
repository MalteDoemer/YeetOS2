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

#include "Types.hpp"
#include "StdLib.hpp"

#include "Kernel/KernelTests.hpp"

namespace Kernel::Tests {

TEST_CASE(memcpy_overrun)
{
    char magic_byte = 0xaa;
    size_t test_size = 32;
    size_t total_size = test_size * 4;

    const char* src
        = "This is a very good memcyp "
          "test------------------------------------------------------------------------------------------------";
    char* mem = new char[total_size];

    for (size_t i = test_size; i < total_size; i++) { mem[i] = magic_byte; }

    memcpy(mem, src, test_size);

    for (size_t i = test_size; i < total_size; i++) {
        EXPECT_EQU(mem[i], magic_byte);    
    }

    return true;
}

TEST_CASE(memmov_underrun)
{
    char magic_byte = 0xaa;
    size_t test_size = 32;
    size_t total_size = test_size * 4;

    // src < mem according to the linker script
    // so we need to test for a "underrun"

    const char* data = "-----------------------------------------------------------------------------------------------"
                       "-This is a very good memmov test";
    const char* src = data + total_size - test_size;

    char* mem = new char[total_size];
    char* dest = mem + total_size - test_size;

    for (size_t i = 0; i < total_size - test_size; i++) { mem[i] = magic_byte; }

    memmov(dest, src, test_size);

    for (size_t i = 0; i < total_size - test_size; i++) {
        EXPECT_EQU(mem[i], magic_byte);
    }

    return true;
}

TEST_CASE(memset_overrun)
{
    char magic_byte = 0xaa;
    size_t test_size = 32;
    size_t total_size = test_size * 4;

    char* mem = new char[total_size];

    for (size_t i = test_size; i < total_size; i++) { mem[i] = magic_byte; }

    memset(mem, '-', test_size);

    for (size_t i = test_size; i < total_size; i++) {
        EXPECT_EQU(mem[i], magic_byte);
    }

    return true;
}


TEST_CASE(memcpy_content)
{
    const char* data = "Here is some string data to test";

    size_t size = strlen(data);
    char* mem = new char[size];

    memcpy(mem, data, size);

    for (size_t i = 0; i < size; i++) {
        EXPECT_EQU(mem[i], data[i]);
    }
    return true;
}

TEST_CASE(memmov_content)
{
    const char* data = "Here is some string data to test";

    size_t size = strlen(data);
    char* mem = new char[size];

    memmov(mem, data, size);

    for (size_t i = 0; i < size; i++) {
        EXPECT_EQU(mem[i], data[i]);
    }
    return true;
}

TEST_CASE(memset_content)
{
    size_t size = 32;
    char* mem = new char[size];

    memset(mem, 'f', size);

    for (size_t i = 0; i < size; i++) {
        EXPECT_EQU(mem[i], 'f');
    }
    return true;
}

TEST_CASE(strlen_results)
{
    size_t max_size = 1024;
    char* data = new char[max_size + 1];
    memset(data, 'f', max_size);

    for (size_t i = max_size; i <= max_size && i >= 0; i--){
        data[i] = '\0';
        EXPECT_EQU(strlen(data), i);
    }

    return true;
}

}