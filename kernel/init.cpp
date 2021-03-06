
#include "stdint.h"
#include "Multiboot.h"
#include "Heap.h"

typedef void (*ctor_t)();

extern ctor_t start_ctor;
extern ctor_t end_ctor;

bool initialized = false;

void ctor_test() __attribute__((__constructor__));

void ctor_test()
{
    initialized = true;
}

extern "C" void kernel_main()
{
    MultibootInfo::initialize();
    Heap::initialize();

    // call all global constructors
    for (ctor_t* ctor = &start_ctor; ctor < &end_ctor; ctor++) {
        (*ctor)();
    }

}