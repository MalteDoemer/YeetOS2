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
#include "Arch/x86/Interrupts.hpp"

#include "Kernel/Panic.hpp"
#include "Kernel/Kernel.hpp"
#include "Kernel/SerialDebug.hpp"

namespace Kernel::Arch {

typedef void (*CtorFunc)();

LINKER_SYMBOL(ctors_start);
LINKER_SYMBOL(ctors_end);

ASM_LINKAGE bool init_sse();

void call_ctors()
{
    /* Ehhmm clang tried to optimize this comparison further down.
       Now I need to do this void* stuff in order to prevent it.   */
    void* start = (void*)ctors_start;
    void* end = (void*)ctors_end;

    if (start == end) {
        return;
    }

    CtorFunc* ctor = (CtorFunc*)end;

    do {
        ctor--;
        (*ctor)();
    } while (ctor > (CtorFunc*)start);
}

void initialize()
{
    if (!init_sse()) {
        Serial::println("SSE not supported!");
    }

    init_gdt();
    init_idt();
    init_ints();
}

}