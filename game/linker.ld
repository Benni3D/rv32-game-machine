ENTRY(_start)

MEMORY {
   ROM  (rx) : ORIGIN = 0, LENGTH = 64K
   SRAM (wx) : ORIGIN = 64K, LENGTH = 64K
   RAM  (wx) : ORIGIN = 0x10000000, LENGTH = 1M
}

SECTIONS {
   . = 0;
   .text : {
      *(.boot)
      *(.text)
   }
   .rodata : {
      *(.rodata)
   }
   .data : {
      *(.data)
   }
   .bss : {
      *(COMMON)
      *(.bss)
   }
}
