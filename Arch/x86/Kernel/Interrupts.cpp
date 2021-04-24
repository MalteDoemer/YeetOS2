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
#include "Platform.hpp"
#include "Functions.hpp"

#include "Arch/x86/Asm.hpp"
#include "Arch/x86/Interrupts.hpp"

#include "Kernel/Panic.hpp"
#include "Kernel/SerialDebug.hpp"

namespace Kernel::Arch {

constexpr u8 pic_intr_vector = 0x20;
static Function<void(Registers*)> handlers[256];

void remap_pic()
{
    // start initialization
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // set the vector address (int 0x20 - 0x2F)
    outb(0x21, pic_intr_vector);
    outb(0xA1, pic_intr_vector + 8);

    // pic1 is master and pic2 is slave
    outb(0x21, 4);
    outb(0xA1, 2);

    // 8086/8088 mode
    outb(0x21, 0x1);
    outb(0xA1, 0x1);

    // disable all interrupts
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}

void mask_irq(u8 irq, bool mask)
{
    u8 pic1_mask = inb(0x21);
    u8 pic2_mask = inb(0xA1);

    if (irq < 8) {
        pic1_mask = mask ? pic1_mask | (1 << irq) : pic1_mask & ~(1 << irq);
    } else {
        pic2_mask = mask ? pic2_mask | (1 << (irq - 8)) : pic2_mask & ~(1 << (irq - 8));
    }

    outb(0x21, pic1_mask);
    outb(0xA1, pic2_mask);
}

ASM_LINKAGE void isr_handler(Registers* regs)
{
    if (handlers[regs->int_num]) {
        handlers[regs->int_num](regs);
    } else {
        panic("Unhandled Interrupt: int %#x", regs->int_num);
    }
}

void set_intr_handler(u8 int_num, Function<void(Registers*)> handler)
{
    handlers[int_num] = move(handler);
}

void init_ints()
{
    remap_pic();

    auto default_exception_handler = [](Registers* regs) {
        panic("\nUnhandled Kernel Excpetion: %#x, Error Code: %#x\n", regs->int_num, regs->err_code);
    };

    for (u32 i = 0; i < 32; i++) {
        set_intr_handler(i, default_exception_handler);
    }

}

}