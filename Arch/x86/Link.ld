SECTIONS {
   . = 0x100000;

   .mboot : {
      *(.mboot)
   }
   . += 0xC0000000;

   .text : AT(ADDR(.text) - 0xC0000000) {
      _code = . - 0xC0000000;
      *(.text)

      . = ALIGN(8);
      ctors_start = .;
      *(.ctors)
      ctors_end = .;

      *(.rodata*)
   }

   .data ALIGN (4K) : AT(ADDR(.data) - 0xC0000000) {
      *(.data)
   }

   .bss ALIGN (4K) (NOLOAD) : AT(ADDR(.bss) - 0xC0000000) {

      _bss_start = . - 0xC0000000;
      *(COMMON)
      *(.bss)
      _bss_end = . - 0xC0000000;

   
      . = ALIGN(4K);

      *(.page_structs)
      *(.heap)
   }


   _kernel_end_virt = .;
   _kernel_end_phys = . - 0xC0000000;
   _kernel_size = . - 0xC0100000;
   _kernel_size_a = ALIGN(4M) - 0xC0000000;
}