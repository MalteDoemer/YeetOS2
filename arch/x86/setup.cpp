#include "stdint.h"
#include "multiboot.h"

void arch_main();

extern "C" void arch_main(void* mboot, uint32_t magic, uint32_t esp)
{
    
}