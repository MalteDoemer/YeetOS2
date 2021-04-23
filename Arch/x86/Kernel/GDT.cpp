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

#include "Arch/x86/GDT.hpp"
#include "Arch/x86/Arch.hpp"

namespace Kernel::Arch {

ASM_LINKAGE void load_gdt(GDTReference* gdtr);
ASM_LINKAGE void load_tss(u16 descriptor);

static TaskStateSegment tss;
static GDTReference gdtr;
static GDTEntry gdt[6];

void init_gdt()
{
    gdt[0] = GDTEntry(0, 0, 0, 0);
    gdt[1] = GDTEntry(0, 0xFFFFF, 0x9A, 0x0C);
    gdt[2] = GDTEntry(0, 0xFFFFF, 0x92, 0x0C);
    gdt[3] = GDTEntry(0, 0xFFFFF, 0xFA, 0x0C);
    gdt[4] = GDTEntry(0, 0xFFFFF, 0xF2, 0x0C);
    gdt[5] = GDTEntry(reinterpret_cast<u32>(&tss), sizeof(tss), 0xE9, 0x00);

    gdtr.offset = reinterpret_cast<u32>(&gdt);
    gdtr.size = sizeof(gdt) - 1;

    tss.ss0 = KERNEL_DATA_DESC;
    tss.iopb = sizeof(tss); /* I don't know what this is or what it does. :( */

    load_gdt(&gdtr);
    load_tss(TSS_DESC);
}

void set_esp0(u32 esp)
{
    tss.esp0 = esp;
}

}