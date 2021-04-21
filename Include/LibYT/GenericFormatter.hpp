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

#include "Types.hpp"
#include "Platform.hpp"
#include "Concepts.hpp"
#include "StdLib.hpp"

namespace YT {

class GenericFormatter {

protected:
    enum FormatOptions {
        None = 0,
        ZeroPad = 1,
        Sign = 2,
        Plus = 4,
        Space = 8,
        Left = 16,
        Prefix = 32,
        Lowercase = 64,
    };

public:
    using OutputFunction = void (*)(char c, char* buffer, size_t index, size_t maxlen);

protected:
    template<IntegralType T, u8 base>
    requires requires { requires base >= 2 && base <= 36; }
    void write_integer(T value, int width, int precision, FormatOptions opts)
    {
        constexpr size_t max_buffer_size = sizeof(T) * __CHAR_BIT__;

        char sign, pad;
        char local_buffer[max_buffer_size];
        const char* digits
            = (opts & Lowercase) ? "0123456789abcdefghjkilmnopqrstuvwxyz" : "0123456789ABCDEFGHJKILMNOPQRSTUVWXYZ";

        /* Left aligned cannot be zero padded */
        if (opts & Left) {
            opts = static_cast<FormatOptions>(opts & ~ZeroPad);
        }

        if (opts & ZeroPad) {
            pad = '0';
        } else {
            pad = ' ';
        }

        if (opts & Sign) {
            using SignedT = typename MakeSigned<T>::Type;

            if (static_cast<SignedT>(value) < 0) {
                sign = '-';
                SignedT s = static_cast<SignedT>(value);
                s *= -1;
                value = static_cast<T>(s);
            } else if (opts & Plus) {
                sign = '+';
            } else if (opts & Space) {
                sign = ' ';
            } else {
                sign = 0;
            }
        } else {
            sign = 0;
        }

        /* Decrease the width if we need to display a sign */
        if (sign) {
            width--;
        }

        /* Decrease the width if there is a prefix (0x, 0o, 0b) */
        if (opts & Prefix) {
            switch (base) {
            case 16:
            case 8:
            case 2:
                width -= 2;
                break;
            default:
                break;
            }
        }

        int i = 0;

        /* Zero is a special case */
        if (value == 0) {
            local_buffer[0] = '0';
            i = 1;
        }

        for (; value; i++) {
            local_buffer[i] = digits[value % base];
            value /= base;
        }

        if (i > precision) {
            precision = i;
        }

        width -= precision;

        if (!(opts & ZeroPad) && !(opts & Left)) {
            while (width > 0) {
                m_out_func(' ', m_buffer, m_index++, m_maxlen);
                width--;
            }
        }

        if (sign) {
            m_out_func(sign, m_buffer, m_index++, m_maxlen);
        }

        if (opts & Prefix) {
            if (base == 16) {
                m_out_func('0', m_buffer, m_index++, m_maxlen);
                m_out_func('x', m_buffer, m_index++, m_maxlen);
            } else if (base == 8) {
                m_out_func('0', m_buffer, m_index++, m_maxlen);
                m_out_func('o', m_buffer, m_index++, m_maxlen);
            } else if (base == 2) {
                m_out_func('0', m_buffer, m_index++, m_maxlen);
                m_out_func('b', m_buffer, m_index++, m_maxlen);
            }
        }

        if (!(opts & Left)) {
            while (width > 0) {
                m_out_func(pad, m_buffer, m_index++, m_maxlen);
                width--;
            }
        }

        while (i < precision) {
            m_out_func('0', m_buffer, m_index++, m_maxlen);
            precision--;
        }

        while (i) {
            m_out_func(local_buffer[--i], m_buffer, m_index++, m_maxlen);
        }

        while (width > 0) {
            m_out_func(' ', m_buffer, m_index++, m_maxlen);
            width--;
        }
    }

    template<FloatingPointType T> 
    void write_float(T value, int width, int precision, FormatOptions opts)
    {
        VERIFY_NOT_REACHED();
    }

    void write_string(const char* str, int width, int precision, FormatOptions opts)
    {
        size_t len;

        if (precision > 0) {
            len = strnlen(str, precision);
        } else {
            len = strlen(str);
        }

        width -= len;

        if (!(opts & Left)) {
            while (width > 0) {
                m_out_func(' ', m_buffer, m_index++, m_maxlen);
                width--;
            }
        }

        while (len--) {
            m_out_func(*str++, m_buffer, m_index++, m_maxlen);
        }

        while (width > 0) {
            m_out_func(' ', m_buffer, m_index++, m_maxlen);
            width--;
        }
    }

    void write_char(char c, int width, int precision, FormatOptions opts)
    {
        if (!(opts & Left))
        {
            while (width > 0) {
                m_out_func(' ', m_buffer, m_index++, m_maxlen);
                width--;
            }

            m_out_func(c, m_buffer, m_index++, m_maxlen);

            while (width > 0) {
                m_out_func(' ', m_buffer, m_index++, m_maxlen);
                width--;
            }

        }
    }

protected:
    OutputFunction m_out_func;
    char* m_buffer { nullptr };
    size_t m_index { 0 };
    size_t m_maxlen { 0 };
};
}

using YT::GenericFormatter;