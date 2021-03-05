
#include "stdint.h"
#include "stddef.h"

#include "yeetos/kernel.h"

void setup_arch(void* mboot, uint32_t magic, uint32_t esp)
{
    uint16_t* vram = (uint16_t*) (0xB8000 + 0xC0000000);
    vram[0] = 0x1F20;

    setup_kernel();
}