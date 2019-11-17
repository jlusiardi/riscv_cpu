OUTPUT_FORMAT(elf32-littleriscv)

ENTRY(_start);

MEMORY {
  rom (rx) : ORIGIN = 0x00000000, LENGTH = 2048
  ram (wx) : ORIGIN = 0x00000400, LENGTH = 4096
}

SECTIONS {
    .text : {
            *(.text.prologue);
            *(.text*);
        } > rom
}