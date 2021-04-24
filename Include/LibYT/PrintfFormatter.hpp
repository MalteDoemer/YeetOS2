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

#include "stdarg.h"

#include "Types.hpp"
#include "GenericFormatter.hpp"

namespace YT {

#define CALL_WRITE_INTEGER(base)                                                                                       \
    do {                                                                                                               \
        switch (length) {                                                                                              \
        case LengthModifier::None:                                                                                     \
            write_integer<unsigned int, base>((unsigned int)(va_arg(vargs, int)), width, precision, opts);             \
            break;                                                                                                     \
        case LengthModifier::Char:                                                                                     \
            write_integer<unsigned char, base>((unsigned char)(va_arg(vargs, int)), width, precision, opts);           \
            break;                                                                                                     \
        case LengthModifier::Short:                                                                                    \
            write_integer<unsigned short, base>((unsigned short)(va_arg(vargs, int)), width, precision, opts);         \
            break;                                                                                                     \
        case LengthModifier::Long:                                                                                     \
            write_integer<unsigned long, base>((unsigned long)(va_arg(vargs, long)), width, precision, opts);          \
            break;                                                                                                     \
        case LengthModifier::LongLong:                                                                                 \
            write_integer<unsigned long long, base>(                                                                   \
                (unsigned long long)(va_arg(vargs, long long)), width, precision, opts);                               \
            break;                                                                                                     \
        default:                                                                                                       \
            VERIFY_NOT_REACHED();                                                                                      \
        }                                                                                                              \
    } while (false)

class PrintfFormatter : public GenericFormatter {

    enum class LengthModifier { None, Char, Short, Long, LongLong };

public:
    PrintfFormatter(OutputFunction out_func, char* buffer, size_t maxlen)
    {
        m_out_func = move(out_func);
        m_buffer = buffer;
        m_index = 0;
        m_maxlen = maxlen;
    }

    size_t format(const char* fmt, va_list vargs)
    {
        /* %[flags][width][.precision][length]specifier */

        while (*fmt) {
            if (*fmt != '%') {
                m_out_func(*fmt, m_buffer, m_index++, m_maxlen);
                fmt++;
                continue;
            }

            fmt++;

            FormatOptions opts = None;

        eval_flags:
            switch (*fmt) {
            case '0':
                opts = (FormatOptions)(opts | ZeroPad);
                fmt++;
                goto eval_flags;
            case '-':
                opts = (FormatOptions)(opts | Left);
                fmt++;
                goto eval_flags;
            case '+':
                opts = (FormatOptions)(opts | Plus);
                fmt++;
                goto eval_flags;
            case ' ':
                opts = (FormatOptions)(opts | Space);
                fmt++;
                goto eval_flags;
            case '#':
                opts = (FormatOptions)(opts | Prefix);
                fmt++;
                goto eval_flags;
            default:
                break;
            }

            int width = 0;
            if (isdigit(*fmt)) {
                width = atoi_skip(&fmt);
            } else if (*fmt == '*') {
                int w = va_arg(vargs, int);
                if (w < 0) {
                    opts = (FormatOptions)(opts | Left);
                    width = -w;
                } else {
                    width = w;
                }
                fmt++;
            }

            int precision = 0;
            if (*fmt == '.') {
                fmt++;

                if (isdigit(*fmt)) {
                    precision = atoi_skip(&fmt);
                } else if (*fmt == '*') {
                    precision = va_arg(vargs, int);
                    fmt++;
                }
            }

            LengthModifier length = LengthModifier::None;

            if (*fmt == 'h') {
                if (*++fmt == 'h') {
                    length = LengthModifier::Char;
                    fmt++;
                } else {
                    length = LengthModifier::Short;
                }
            } else if (*fmt == 'l') {
                if (*++fmt == 'l') {
                    length = LengthModifier::LongLong;
                    fmt++;
                } else {
                    length = LengthModifier::Long;
                }
            }

            switch (*fmt) {
            case 'c':
                write_char((char)va_arg(vargs, int), width, precision, opts);
                fmt++;
                break;

            case 's':
                write_string((const char*)va_arg(vargs, char*), width, precision, opts);
                fmt++;
                break;

            case 'd':
            case 'i':
                opts = (FormatOptions)(opts | Sign);
            case 'u':
                CALL_WRITE_INTEGER(10);
                fmt++;
                break;
            case 'o':
            case 'O':
                CALL_WRITE_INTEGER(8);
                fmt++;
                break;
            case 'b':
            case 'B':
                CALL_WRITE_INTEGER(2);
                fmt++;
                break;
            case 'x':
            case 'X':
                CALL_WRITE_INTEGER(16);
                fmt++;
                break;
            case 'p':
                opts = (FormatOptions)(opts | Prefix);
                precision = sizeof(void*) * 2;
                write_integer<uintptr_t, 16>((uintptr_t)va_arg(vargs, void*), width, precision, opts);
                fmt++;
                break;
            case 'n':
                // TODO: implement %n
                VERIFY_NOT_REACHED();
            }
        }

        m_out_func('\0', m_buffer, m_index < m_maxlen ? m_index : m_maxlen - 1U, m_maxlen);
        return m_index;
    }
};
}

using YT::PrintfFormatter;