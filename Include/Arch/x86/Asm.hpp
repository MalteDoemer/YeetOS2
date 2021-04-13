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

static inline void outb(Uint16 port, Uint8 data)
{
    asm volatile("outb %1, %0" ::"dN"(port), "a"(data));
}

static inline void outw(Uint16 port, Uint16 data)
{
    asm volatile("outw %1, %0" ::"dN"(port), "a"(data));
}

static inline Uint8 inb(Uint16 port)
{
    Uint8 ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)
                 : "dN"(port));
    return ret;
}

static inline Uint16 inw(Uint16 port)
{
    Uint16 ret;
    asm volatile("inw %1, %0"
                 : "=a"(ret)
                 : "dN"(port));
    return ret;
}

static inline void stosb(void* buf, Uint8 val, Uint32 count)
{
    asm("rep stosb" ::"a"(val), "c"(count), "D"(buf));
}

static inline void stosw(void* buf, Uint16 val, Uint32 count)
{
    asm("rep stosw" ::"a"(val), "c"(count), "D"(buf));
}

static inline void stosd(void* buf, Uint32 val, Uint32 count)
{
    asm("rep stosl" ::"a"(val), "c"(count), "D"(buf));
}

static inline void movsb(void* dest, const void* src, Uint32 count)
{
    asm("rep movsb" :: "D"(dest), "S"(src), "c"(count));
}

static inline void movsw(void* dest, const void* src, Uint32 count)
{
    asm("rep movsw" :: "D"(dest), "S"(src), "c"(count));
}

static inline void movsd(void* dest, const void* src, Uint32 count)
{
    asm("rep movsl" :: "D"(dest), "S"(src), "c"(count));
}

static inline void invlpg(FlatPtr addr)
{
    asm("invlpg %0" ::"m"(addr));
}

static inline void set_cr3(Uint32 val)
{
    asm("movl %%eax, %%cr3" ::"a"(val));
}

static inline void cli()
{
    asm("cli");
}

static inline void sti()
{
    asm("sti");
}

static inline void cld()
{
    asm("cld");
}

static inline void setd()
{
    asm("std");
}


static inline void hlt()
{
    asm("hlt");
}
