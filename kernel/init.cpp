
#include "stdint.h"
#include "libc/string.h"
#include "kernel/Multiboot.h"
#include "kernel/Heap.h"


class CtorTest {
public:
    bool initialized = false;
    
    CtorTest()
    {
        initialized = true;
    }
};


// CtorTest t1;
// CtorTest t2;

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

    MultibootInfo::initialize();
    Heap::initialize();

    // call all global constructors
    call_ctors();

    char* c = new char[5];
    memcpy(c, "Fett", 5);
    delete[] c;

    int* i = new int(36);
    char* buff = new char[1024];

    delete i;
    delete[] buff;
}