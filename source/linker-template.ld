ENTRY(gcnhook)
SECTIONS {
  .text @TEXT@ : { KEEP(*(.text .text.*)) }
  .rodata @RODATA@ : { KEEP(*(.rodata .rodata.*)) }
  .data : { *(.data .data.*) }
  .bss (NOLOAD) : { *(.bss .bss.*) }
  /DISCARD/ : { *(.comment) *(.note*) *(.llvm_addrsig) }
}
