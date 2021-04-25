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

    for (size_t i = test_size; i < total_size; i++) {
        mem[i] = magic_byte;
    }

    memcpy(mem, src, test_size);

    for (size_t i = test_size; i < total_size; i++) {
        EXPECT_EQU(mem[i], magic_byte);
    }

    delete[] mem;
    return true;
}

TEST_CASE(memmove_underrun)
{
    char magic_byte = 0xaa;
    size_t test_size = 32;
    size_t total_size = test_size * 4;

    // src < mem according to the linker script
    // so we need to test for a "underrun"

    const char* data = "-----------------------------------------------------------------------------------------------"
                       "-This is a very good memmove test";
    const char* src = data + total_size - test_size;

    char* mem = new char[total_size];
    char* dest = mem + total_size - test_size;

    for (size_t i = 0; i < total_size - test_size; i++) {
        mem[i] = magic_byte;
    }

    memmove(dest, src, test_size);

    for (size_t i = 0; i < total_size - test_size; i++) {
        EXPECT_EQU(mem[i], magic_byte);
    }

    delete[] mem;
    return true;
}

TEST_CASE(memset_overrun)
{
    char magic_byte = 0xaa;
    size_t test_size = 32;
    size_t total_size = test_size * 4;

    char* mem = new char[total_size];

    for (size_t i = test_size; i < total_size; i++) {
        mem[i] = magic_byte;
    }

    memset(mem, '-', test_size);

    for (size_t i = test_size; i < total_size; i++) {
        EXPECT_EQU(mem[i], magic_byte);
    }

    delete[] mem;
    return true;
}

TEST_CASE(memmem_correctness)
{
    const char* str = "Hello this is a very very long string. Actually that is not true, cause there are many strings that are mucho mucho more long XD torolololololol";
    const char* ptr1 = str + 126;
    const char* ptr2 = str + 0;
    const char* ptr3 = str + 129;
    const char* ptr4 = str + 31;
    
    EXPECT_EQU(memmem(str, strlen(str), "XD", 2), ptr1);
    EXPECT_EQU(memmem(str, strlen(str), "Hello", 5), ptr2);
    EXPECT_EQU(memmem(str, strlen(str), "torolololololol", 15), ptr3);
    EXPECT_EQU(memmem(str, strlen(str), "string", 6), ptr4);

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

    delete[] mem;
    return true;
}

TEST_CASE(memmove_content)
{
    const char* data = "Here is some string data to test";

    size_t size = strlen(data);
    char* mem = new char[size];

    memmove(mem, data, size);

    for (size_t i = 0; i < size; i++) {
        EXPECT_EQU(mem[i], data[i]);
    }

    delete[] mem;
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

    delete[] mem;
    return true;
}

TEST_CASE(strlen_results)
{
    size_t max_size = 1024;
    char* data = new char[max_size + 1];
    memset(data, 'f', max_size);

    for (size_t i = max_size; i <= max_size && i >= 0; i--) {
        data[i] = '\0';
        EXPECT_EQU(strlen(data), i);
    }

    delete[] data;
    return true;
}

TEST_CASE(strnlen_resulsts)
{
    size_t max_size = 1024;
    char* data = new char[max_size + 1];
    memset(data, 'f', max_size);

    for (size_t i = max_size; i <= max_size && i >= 0; i--) {
        data[i] = '\0';
        EXPECT_EQU(strnlen(data, max_size), i);

        if (i > 0)
            EXPECT_EQU(strnlen(data, i - 1), i - 1);
    }

    delete[] data;
    return true;
}

TEST_CASE(strcmp_results)
{
    const char* str1 = "haha";
    const char* str2 = "aha";
    const char* str3 = "hehe";
    const char* str4 = "hahahahaha";

    EXPECT(strcmp(str1, str2) > 0);
    EXPECT(strcmp(str2, str3) < 0);
    EXPECT(strcmp(str1, str4) < 0);
    EXPECT_EQU(strcmp("", ""), 0);
    EXPECT_EQU(strcmp(str2, "aha"), 0);
    EXPECT_EQU(strcmp(str4, str4), 0);

    return true;
}

TEST_CASE(strncmp_results)
{

    const char droids[7][6] = { "R2-D2", "C3-PO", "R2-A6", "R3-S6", "R2-KT", "R7-A6", "R2-A3" };

    size_t r2_count = 0;
    for (size_t i = 0; i < 7; i++) {
        if (strncmp(droids[i], "R2-XX", 3) == 0)
            r2_count++;
    }

    EXPECT_EQU(r2_count, 4ul);

    EXPECT_EQU(strncmp("lol", "lololol", 3), 0);
    EXPECT(strncmp("heyy", "heyhey", 4) > 0);

    return true;
}

}