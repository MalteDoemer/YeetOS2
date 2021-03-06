SECTIONS {
   . = 0x100000;

   .mboot : {
      *(.mboot)
   }

   . += 0xC0000000;

   .text : AT(ADDR(.text) - 0xC0000000) {
      code = . - 0xC0000000;
      *(.text)

      *Heap.o(.ctors)

      start_ctor = .;
      *(.ctors)
      end_ctor = .;

      *(.rodata*)
   }

   .data ALIGN (4K) : AT(ADDR(.data) - 0xC0000000) {
      *(.data)
   }

   .bss ALIGN (4K) : AT(ADDR(.bss) - 0xC0000000) {
      bss_start = . - 0xC0000000;

      *(COMMON)
      *(.bss)

      heap_start = .;
      . += 0x400000;
      heap_end = .;

      bss_end = . - 0xC0000000;
   }

   /* kernel_end = .; */
   /* kernel_size = kernel_end - 0xC0100000; */
}