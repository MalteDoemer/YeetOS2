ENTRY(start)

SECTIONS {
   . = 0x100000;

   .mboot : {
      load_start = .;
      *(.mboot)
   }
   . += 0xC0000000;

   .text : AT(ADDR(.text) - 0xC0000000) {
      *(.text*)
   }

   .rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000) {
      ctors_start = .;
      *(.init_array)
      *(.ctors)
      ctors_end = .;
      *(.rodata*)
   }

   .data ALIGN (4K) : AT(ADDR(.data) - 0xC0000000) {
      *(.data*)
      load_end = . - 0xC0000000;
   }

   .bss ALIGN (4K) (NOLOAD) : AT(ADDR(.bss) - 0xC0000000) {
      *(COMMON)
      *(.bss*)
      bss_end = . - 0xC0000000;
   }

   .heap_memory ALIGN (4M) (NOLOAD) : AT(ADDR(.heap_memory) - 0xC0000000) {
      *(.heap_memory)
   }

   _kernel_end_virt = .;
   _kernel_end_phys = . - 0xC0000000;
   _kernel_size = . - 0xC0100000;
   _kernel_size_a = ALIGN(4M) - 0xC0000000;
}