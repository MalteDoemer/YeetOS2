#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdarg.h"

#define FLAGS_ZEROPAD 1  /* pad with zeroS */
#define FLAGS_SIGN 2     /* unsigned/signed */
#define FLAGS_PLUS 4     /* show plus positive */
#define FLAGS_SPACE 8    /* space if positive */
#define FLAGS_LEFT 16    /* left justified */
#define FLAGS_SPECIAL 32 /* 0x,0o,0b */
#define FLAGS_SMALL 64   /* use lowercase digits */
// #define HAS_WIDTH 128    /* width field was specified */
// #define HAS_PREC 256     /* precision field was specified */

#define TYPE_NONE 0
#define TYPE_CHAR 1
#define TYPE_SHORT 2
#define TYPE_LONG 4
#define TYPE_LONG_LONG 8

typedef void (*out_func_t)(char c, void* buffer, size_t index, size_t maxlen);

static void out_buffer(char c, void* buffer, size_t index, size_t maxlen)
{
    char* str = (char*)buffer;

    if (index > maxlen)
        return;

    str[index] = c;
}

static void out_null(char c, void* buffer, size_t index, size_t maxlen)
{
    (void)c;
    (void)buffer;
    (void)index;
    (void)maxlen;
}

static size_t strlen_s(const char* str, size_t maxlen)
{
    size_t i = 0;

    while (str[i] && i < maxlen) {
        i++;
    }

    return i;
}

static inline bool is_digit(char c)
{
    return (c >= '0') && (c <= '9');
}

static size_t atoi_skip(const char** s)
{
    size_t i = 0;

    while (is_digit(**s)) {
        i = i * 10 + **s - '0';
        (*s)++;
    }

    return i;
}

static size_t fromat_number32(
    out_func_t out,
    char* buffer,
    size_t index,
    size_t maxlen,
    uint32_t value,
    uint32_t base,
    int width,
    int precision,
    uint32_t flags)
{
    char sign, pad;
    char tmp[36];
    size_t i;
    const char* digits;

    digits = (flags & FLAGS_SMALL) ? "0123456789abcdefghjkilmnopqrstuvwxyz" : "0123456789ABCDEFGHJKILMNOPQRSTUVWXYZ";

    if (base < 2 || base > 36) {
        return 0;
    }

    if (flags & FLAGS_LEFT) {
        flags &= ~FLAGS_ZEROPAD;
    }

    pad = (flags & FLAGS_ZEROPAD) ? '0' : ' ';

    if (flags & FLAGS_SIGN) {
        if ((int32_t)value < 0) {
            sign = '-';
            int32_t s = (int32_t)value;
            s *= -1;
            value = (uint32_t)s;
        } else if (flags & FLAGS_PLUS) {
            sign = '+';
        } else if (flags & FLAGS_SPACE) {
            sign = ' ';
        }
    } else {
        sign = 0;
    }

    if (sign) {
        width--;
    }

    if (flags & FLAGS_SPECIAL) {
        if (base == 16) {
            width -= 2;
        } else if (base == 2) {
            width -= 2;
        } else if (base == 8) {
            width -= 1;
        }
    }

    if (value == 0)
        tmp[0] == '0';

    for (i = 0; value; i++) {
        tmp[i] = digits[value % base];
        value = value / base;
    }

    if (i > precision) {
        precision = i;
    }

    width -= precision;

    if (!(flags & FLAGS_ZEROPAD) && !(flags & FLAGS_LEFT)) {
        while (width > 0) {
            out(' ', buffer, index++, maxlen);
            width--;
        }
    }

    if (sign) {
        out(sign, buffer, index++, maxlen);
    }

    if (flags & FLAGS_SPECIAL) {
        if (base == 16) {
            out('0', buffer, index++, maxlen);
            out('x', buffer, index++, maxlen);
        } else if (base == 8) {
            out('0', buffer, index++, maxlen);
            out('o', buffer, index++, maxlen);
        } else if (base == 2) {
            out('0', buffer, index++, maxlen);
            out('b', buffer, index++, maxlen);
        }
    }

    if (!(flags & FLAGS_LEFT)) {
        while (width > 0) {
            out(pad, buffer, index++, maxlen);
            width--;
        }
    }

    while (i < precision) {
        out('0', buffer, index++, maxlen);
        precision--;
    }

    while (i) {
        out(tmp[--i], buffer, index++, maxlen);
    }

    while (width > 0) {
        out(' ', buffer, index++, maxlen);
        width--;
    }

    return index;
}

size_t vsnprintf_impl(out_func_t out, char* buffer, size_t maxlen, const char* fmt, va_list va)
{
    uint32_t flags = 0;
    uint32_t type = 0;
    int precision = 0;
    int width = 0;
    size_t index = 0;

    const char* s;
    size_t len;
    int* iptr;

    // allows user to specifie NULL as buffer
    // in order to get the number of chars
    if (!buffer) {
        out = out_null;
    }

    while (*fmt) {

        // check for fomat specifier
        // %[flags][width][.precision][length]specifier

        if (*fmt != '%') {
            out(*fmt, buffer, index++, maxlen);
            fmt++;
            continue;
        }

        fmt++;

        // evaluate the flags field
    eval_flags:
        switch (*fmt) {
        case '0':
            flags |= FLAGS_ZEROPAD;
            fmt++;
            goto eval_flags;
        case '-':
            flags |= FLAGS_LEFT;
            fmt++;
            goto eval_flags;
        case '+':
            flags |= FLAGS_PLUS;
            fmt++;
            goto eval_flags;
        case ' ':
            flags |= FLAGS_SPACE;
            fmt++;
            goto eval_flags;
        case '#':
            flags |= FLAGS_SPECIAL;
            fmt++;
            goto eval_flags;
        default:
            break;
        }

        // evaluate the width field
        if (is_digit(*fmt)) {
            width = atoi_skip(&fmt);
        } else if (*fmt == '*') {
            int w = va_arg(va, int);
            if (w < 0) {
                flags |= FLAGS_LEFT;
                width = -w;
            } else {
                width = w;
            }
            fmt++;
        }

        // evalutate the precision field
        if (*fmt == '.') {
            fmt++;

            if (is_digit(*fmt)) {
                precision = atoi_skip(&fmt);
            } else if (*fmt == '*') {
                precision = va_arg(va, int);
                fmt++;
            }
        }

        // evaluate the length field (not used yet)
        switch (*fmt) {
        case 'h':
            if (*++fmt == 'h') {
                type = TYPE_CHAR;
                fmt++;
            } else {
                type = TYPE_SHORT;
            }
            break;
        case 'l':
            if (*++fmt == 'l') {
                type = TYPE_LONG_LONG;
                fmt++;
            } else {
                type = TYPE_LONG;
            }
            break;
        case 'j':
            type = (sizeof(intmax_t) == sizeof(long long)) ? TYPE_LONG_LONG : TYPE_LONG;
            fmt++;
            break;
        case 'z':
            type = (sizeof(size_t) == sizeof(long long)) ? TYPE_LONG_LONG : TYPE_LONG;
            fmt++;
            break;
        case 't':
            type = (sizeof(ptrdiff_t) == sizeof(long long)) ? TYPE_LONG_LONG : TYPE_LONG;
            fmt++;
            break;
        default:
            type = TYPE_NONE;
            break;
        }

        switch (*fmt) {
        case 'c':
            while (!(flags & FLAGS_LEFT) && width > 0) {
                out(' ', buffer, index++, maxlen);
                width--;
            }

            out((char)va_arg(va, int), buffer, index++, maxlen);

            while (width > 0) {
                out(' ', buffer, index++, maxlen);
                width--;
            }
            fmt++;
            break;

        case 's':
            s = va_arg(va, char*);
            len = strlen_s(s, precision > 0 ? (size_t)precision : (size_t)-1);
            // TODO: unsigned/signed check
            width -= len;

            while (!(flags & FLAGS_LEFT) && width > 0) {
                out(' ', buffer, index++, maxlen);
                width--;
            }

            for (size_t i = 0; i < len; i++) {
                out(*s++, buffer, index++, maxlen);
            }

            while (width > 0) {
                out(' ', buffer, index++, maxlen);
                width--;
            }
            fmt++;
            break;

        case 'd':
        case 'i':
            flags |= FLAGS_SIGN;
        case 'u':
            index = fromat_number32(out, buffer, index, maxlen, va_arg(va, uint32_t), 10, width, precision, flags);
            fmt++;
            break;

        case 'o':
            flags |= FLAGS_SMALL;
        case 'O':
            index = fromat_number32(out, buffer, index, maxlen, va_arg(va, uint32_t), 8, width, precision, flags);
            fmt++;
            break;

        case 'b':
            flags |= FLAGS_SMALL;
        case 'B':
            index = fromat_number32(out, buffer, index, maxlen, va_arg(va, uint32_t), 2, width, precision, flags);
            fmt++;
            break;

        case 'x':
            flags |= FLAGS_SMALL;
        case 'X':
            index = fromat_number32(out, buffer, index, maxlen, va_arg(va, uint32_t), 16, width, precision, flags);
            fmt++;
            break;

        case 'p':
            if (!(flags & FLAGS_LEFT)) {
                width = 0;
            }

            flags |= FLAGS_SPECIAL;
            precision = sizeof(void*) * 2;
            index = fromat_number32(out, buffer, index, maxlen, va_arg(va, uint32_t), 16, width, precision, flags);
            fmt++;
            break;

        case 'n':
            iptr = va_arg(va, int*);
            *iptr = index;
            fmt++;
            break;

        case '%':
            out('%', buffer, index++, maxlen);
            fmt++;
            break;

        default:
            out(*fmt++, buffer, index++, maxlen);
            break;
        }
    }

    out('\0', buffer, index < maxlen ? index : maxlen - 1U, maxlen);
    return index;
}

int sprintf(char* buffer, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int ret = (int)vsnprintf_impl(out_buffer, buffer, (size_t)-1, fmt, va);
    va_end(va);
    return ret;
}

int snprintf(char* buffer, size_t count, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int ret = (int)vsnprintf_impl(out_buffer, buffer, count, fmt, va);
    va_end(va);
    return ret;
}

int vsprintf(char* buffer, const char* fmt, va_list va)
{
    return (int)vsnprintf_impl(out_buffer, buffer, (size_t)-1, fmt, va);
}

int vsnprintf(char* buffer, size_t count, const char* fmt, va_list va)
{
    return (int)vsnprintf_impl(out_buffer, buffer, count, fmt, va);
}
