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
#include "Arch/x86/IDT.hpp"
#include "Arch/x86/Registers.hpp"

#include "Kernel/SerialDebug.hpp"

namespace Kernel::Arch {

ASM_LINKAGE void load_idt(IDTReference* idtr);

ASM_LINKAGE ISRStub isr_stubs[256];

static IDTEntry idt[256];
static IDTReference idtr;

void init_idt()
{
    idtr.offset = reinterpret_cast<u32>(&idt);
    idtr.size = sizeof(idt) - 1;

    for (size_t i = 0; i < 256; i++) {
        idt[i] = IDTEntry(reinterpret_cast<u32>(&isr_stubs[i]), KERNEL_CODE_DESC, 0x8E);
    }

    load_idt(&idtr);
}

ASM_LINKAGE void isr_handler(Registers* regs)
{
    Serial::printf("Interrupt %#x was called\n", regs->int_num);
}

}
