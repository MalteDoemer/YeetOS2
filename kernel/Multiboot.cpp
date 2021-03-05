
#include "Multiboot.h"

void MultibootInfo::initialize()
{

    if (flags & MULTIBOOT_FLAGS_CMDLINE)
        this->cmdline += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_MODULES) {
        this->mods_addr += KERNEL_BASE;

        MultibootModule* mod = (MultibootModule*)this->mods_addr;

        for (uint32_t i = 0; i < this->mods_count; i++) {
            if (mod[i].string)
                mod[i].string += KERNEL_BASE;
        }
    }

    if (flags & MULTIBOOT_FLAGS_MMAP)
        this->mmap_addr += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_DRIVES)
        this->drives_addr += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_CONFIG)
        this->rom_config += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_NAME)
        this->boot_loader_name += KERNEL_BASE;
}

