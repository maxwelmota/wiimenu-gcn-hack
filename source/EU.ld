ENTRY(gcnhook)
SECTIONS {
  .text 0x8135A244 : { KEEP(*(.text .text.*)) }
  .rodata 0x81359F40 : { KEEP(*(.rodata .rodata.*)) }
  .data : { *(.data .data.*) }
  .bss (NOLOAD) : { *(.bss .bss.*) }
  /DISCARD/ : { *(.comment) *(.note*) *(.llvm_addrsig) }
}
