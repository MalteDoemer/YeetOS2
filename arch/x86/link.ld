SECTIONS {
    . = 0x100000;

    .mboot : {
        *(.mboot)
    }

    . += 0xC0000000;

    .text : AT(ADDR(.text) - 0xC0000000) {
       code = . - 0xC0000000;
       *(.text)
       *(.rodata*)
    }

    .data ALIGN (4K) : AT(ADDR(.data) - 0xC0000000) {
       *(.data)
    }

    .bss ALIGN (4K) : AT(ADDR(.bss) - 0xC0000000) {
       bss = . - 0xC0000000;
       *(COMMON)
       *(.bss)
       end = . - 0xC0000000;
    }


    __page_structs_start = 0xFFC00000;
    __page_structs_end = 0x100000000;
}