ENTRY(gcnhook)
SECTIONS {
  .text 0x8135965C : { KEEP(*(.text .text.*)) }
  .rodata 0x81359358 : { KEEP(*(.rodata .rodata.*)) }
  .data : { *(.data .data.*) }
  .bss (NOLOAD) : { *(.bss .bss.*) }
  /DISCARD/ : { *(.comment) *(.note*) *(.llvm_addrsig) }
}
