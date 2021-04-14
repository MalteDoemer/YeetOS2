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

extern "C" void* memcpy(void* dest, const void* src, size_t n)
{
    asm("rep movsb" : : "S"(src), "D"(dest), "c"(n) : "memory");
    return dest;
}

extern "C" void* memmov(void* dest, const void* src, size_t n)
{
    if (dest > src) {
        size_t dest_end = reinterpret_cast<size_t>(dest) + n - 1;
        size_t src_end = reinterpret_cast<size_t>(src) + n - 1;

        asm("std\n\t"
            "rep movsb\n\t"
            "cld\n\t"
            :
            : "S"(src_end), "D"(dest_end), "c"(n)
            : "memory");

    } else {
        asm("rep movsb" : : "S"(src), "D"(dest), "c"(n) : "memory");
    }

    return dest;
}

extern "C" void memset(void* dest, int c, size_t count) 
{
    size_t value = c & 0xFF;
    


}

extern "C" int memcmp(const void* p1, const void* p2, size_t n) {}

extern "C" size_t strlen(const char* str) {}

extern "C" size_t strnlen(const char* str, size_t maxlen) {}

extern "C" int strcmp(const char* str1, const char* str2) {}

extern "C" int strncmp(const char* str1, const char* str2, size_t n) {}
