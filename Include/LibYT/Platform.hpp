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

#undef ASM_LINKAGE
#define ASM_LINKAGE extern "C"

#undef C_LINKAGE
#define C_LINKAGE extern "C"

#undef ALWAYS_INLINE
#define ALWAYS_INLINE [[gnu::always_inline]] inline

#undef NEVER_INLINE
#define NEVER_INLINE [[gnu::noinline]]

#undef FLATTEN
#define FLATTEN [[gnu::flatten]]

#undef PACKED
#define PACKED [[gnu::packed]]

#undef ALGINED
#define ALIGNED(x) [[gnu::aligned(x)]]

#undef SECTION
#define SECTION(x) [[gnu::section(x)]]

#undef NORETURN
#define NORETURN [[noreturn]]

#undef SYMBOL_VALUE
#define SYMBOL_VALUE(x)                                                                                                \
    ({                                                                                                                 \
        extern char x;                                                                                                 \
        (FlatPtr) & x;                                                                                                 \
    })

constexpr auto is_little_endian = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;
constexpr auto is_bit_endian = __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__;
