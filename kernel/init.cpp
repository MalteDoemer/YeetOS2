
#include "stdint.h"
#include "Multiboot.h"

MultibootInfo* multiboot_ptr;
uint32_t multiboot_sig;

extern "C" void kernel_main()
{
    multiboot_ptr->initialize();
}