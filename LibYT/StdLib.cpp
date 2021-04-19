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

#ifdef __KERNEL__

#include "Kernel/Kheap.hpp"

extern "C" {

void* malloc(size_t size)
{
    return Kernel::Kheap::allocate(size);
}

void* realloc(void* ptr, size_t size)
{
    return Kernel::Kheap::reallocate(ptr, size);
}

void free(void* ptr)
{
    Kernel::Kheap::deallocate(ptr);
}

size_t strlen(const char* str)
{
    const char* start = str;
    while (*str) { str++; }
    return str - start;
}

size_t strnlen(const char* str, size_t maxlen)
{
    size_t counter = maxlen;
    while (*str && counter) { counter--, str++; }
    return maxlen - counter;
}

int strcmp(const char* str1, const char* str2)
{
    for (;; str1++, str2++) {
        if (*str1 != *str2)
            break;

        if (*str1 == '\0' || *str2 == '\0')
            break;
    }

    return *str1 - *str2;
}

int memcmp(const void* p1, const void* p2, size_t n)
{
    const u8* mem1 = static_cast<const u8*>(p1);
    const u8* mem2 = static_cast<const u8*>(p2);

    while (n--) {
        if (*mem1 != *mem2)
            return *mem1 - *mem2;
        mem1++, mem2++;
    }
    return 0;
}

int strncmp(const char* str1, const char* str2, size_t n)
{
    for (; n--; str1++, str2++) {
        if (*str1 != *str2)
            return *str1 - *str2;

        if (*str1 == '\0' || *str2 == '\0')
            return *str1 - *str2;
    }

    return 0;
}
}

#else
#error "userspace not supported!"
#endif