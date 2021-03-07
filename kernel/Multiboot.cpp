
#include "kernel/Multiboot.hpp"

void MultibootInfo::initialize()
{

    uint32_t flags = multiboot_ptr->flags;

    if (flags & MULTIBOOT_FLAGS_CMDLINE)
        multiboot_ptr->cmdline += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_MODULES) {
        multiboot_ptr->mods_addr += KERNEL_BASE;

        MultibootModule* mod = (MultibootModule*)multiboot_ptr->mods_addr;

        for (uint32_t i = 0; i < multiboot_ptr->mods_count; i++) {
            if (mod[i].string)
                mod[i].string += KERNEL_BASE;
        }
    }

    if (flags & MULTIBOOT_FLAGS_MMAP)
        multiboot_ptr->mmap_addr += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_DRIVES)
        multiboot_ptr->drives_addr += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_CONFIG)
        multiboot_ptr->rom_config += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_NAME)
        multiboot_ptr->boot_loader_name += KERNEL_BASE;
}

MultibootInfo* multiboot_ptr;
uint32_t multiboot_sig;
