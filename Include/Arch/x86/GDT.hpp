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

/* Must be in sync with misc.asm */
#define KERNEL_CODE_DESC 0x08
#define KERNEL_DATA_DESC 0x10
#define USER_CODE_DESC 0x1B
#define USER_DATA_DESC 0x23
#define TSS_DESC 0x2B

struct PACKED ALIGNED(8) GDTEntry {
    u16 limit1;
    u16 base1;
    u8 base2;
    u8 access;
    u8 limit2 : 4;
    u8 flags : 4;
    u8 base3;

    GDTEntry() = default;
    GDTEntry(u32 base, u32 limit, u8 access, u8 flags) 
    {
        limit1 = limit & 0xFFFF;
        limit2 = (limit >> 16) & 0xF;

        base1 = base & 0xFFFF;
        base2 = (base >> 16) & 0xFF;
        base3 = (base >> 24) & 0xFF;

        this->access = access;
        this->flags = flags & 0xF;
    }
};

struct PACKED ALIGNED(8) GDTReference {
    u16 size;
    u32 offset;
};

struct PACKED ALIGNED(16) TaskStateSegment {
    u32 link;
    u32 esp0;
    u32 ss0;

    u32 esp1;
    u32 ss1;

    u32 esp2;
    u32 ss2;

    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;

    u32 es;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 fs;
    u32 gs;
    u32 ldtr;
    u32 iopb;
};

namespace Kernel::Arch {

ASM_LINKAGE void load_gdt(GDTReference* gdtr);
ASM_LINKAGE void load_tss(u16 descriptor);

void init_gdt();

}
