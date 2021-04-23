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

#include "New.hpp"
#include "Verify.hpp"
#include "String.hpp"
#include "StdLib.hpp"
#include "UniquePtr.hpp"
#include "Functions.hpp"
#include "StdLibExtras.hpp"

#include "Kernel/Kernel.hpp"
#include "Kernel/Kheap.hpp"
#include "Kernel/Heap.hpp"
#include "Kernel/SerialDebug.hpp"
#include "Kernel/KernelTests.hpp"

namespace Kernel {

void call_fn(Function<void()> func)
{
    func();
}

ASM_LINKAGE void kernel_main()
{
    Kheap::initialize();
    Arch::call_ctors();
    Serial::initialize();
    Arch::initialize();

    Serial::println("");
    Serial::println("+-----------------------------------+");
    Serial::println("| Welcome to the debug spam output! |");
    Serial::println("+-----------------------------------+");
    Serial::println("");

#ifdef __KERNEL_TESTS__
    Kernel::Tests::run_all_tests();
#endif

    // Function<int(int, int)> func = [](int a, int b) -> int
    // {
    //     return a + b;
    // };

    call_fn([]() -> void {
        Serial::println("Heyhey!");
    });
}
}