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

#include "stdarg.h"

#include "PrintfFormatter.hpp"

#include "Arch/x86/Asm.hpp"

#include "Kernel/Kernel.hpp"
#include "Kernel/SerialDebug.hpp"

#define COM1 0x3F8

#define DATA 0
#define IER 1
#define FIFO_CTRL 2
#define LINE_CTRL 3
#define MODEM_CTRL 4
#define LINE_STAT 5
#define MODEM_STAT 6
#define SCRATCH 7

namespace Kernel::Serial {

static inline bool is_transmit_empty()
{
    return inb(COM1 + LINE_STAT) & 0x20;
}

void initialize()
{
    /* disable all serial interrupts */
    outb(COM1 + IER, 0x00);

    /* set divisor to 1 */
    outb(COM1 + LINE_CTRL, 0x80);
    outb(COM1 + 0, 0x01);
    outb(COM1 + 1, 0x00);

    /* set 8N1 */
    outb(COM1 + LINE_CTRL, 0x03);

    /* enable FIFO */
    outb(COM1 + FIFO_CTRL, 0xC7);

    /* set OUT1, OUT2, RTS and DSR to inactive */
    outb(COM1 + MODEM_CTRL, 0x0F);
}

void putchar(char c)
{
    while (!is_transmit_empty()) {}
    outb(COM1 + DATA, c);
}

int print(const char* msg)
{
    int res = 0;
    for (; *msg; msg++) {
        res++;
        putchar(*msg);
    }
    return res;
}

int println(const char* msg)
{
    int res = print(msg);
    putchar('\n');
    return res + 1;
}

static void serial_out_func(char c, char* buf, size_t idx, size_t max)
{
    Serial::putchar(c);
}

int vprintf(const char* fmt, va_list vargs)
{
    PrintfFormatter formatter(serial_out_func, nullptr, -1);
    size_t res = formatter.format(fmt, vargs);
    return (int)res;
}

int printf(const char* fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);

    PrintfFormatter formatter(serial_out_func, nullptr, -1);
    size_t res = formatter.format(fmt, vargs);

    va_end(vargs);

    return (int)res;
}

}