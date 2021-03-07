
#include "stdint.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include "kernel/Multiboot.hpp"
#include "kernel/Heap.hpp"


void call_ctors()
{
    typedef void (*ctor_t)();

    extern ctor_t ctors_start;
    extern ctor_t ctors_end;

    ctor_t* ctor = &ctors_end;

    for (ctor_t* ctor = &ctors_start; ctor < &ctors_end; ctor++)
    {
        (*ctor)();
    }
}


extern "C" void kernel_main()
{

    init_multiboot();
    init_heap();

    // call all global constructors
    call_ctors();
}