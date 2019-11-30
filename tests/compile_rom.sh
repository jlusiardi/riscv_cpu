#!/usr/bin/env bash

. functions.sh

check_for_binary riscv32-unknown-elf-gcc
check_for_binary riscv32-unknown-elf-ld
check_for_binary riscv32-unknown-elf-objcopy
check_for_binary riscv32-unknown-elf-size
check_for_binary riscv32-unknown-elf-objdump


TMPDIR=`mktemp -d`
INPUT="$1"

cat <<EOT >> ${TMPDIR}/link.ld
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
EOT

riscv32-unknown-elf-gcc -c -nostdlib -ffreestanding ${INPUT}.c -o ${TMPDIR}/main.o
riscv32-unknown-elf-gcc -c ${INPUT}.s -o ${TMPDIR}/as.o 
riscv32-unknown-elf-ld -T${TMPDIR}/link.ld ${TMPDIR}/*.o -o ${TMPDIR}/a.out
riscv32-unknown-elf-objcopy -O binary ${TMPDIR}/a.out ${INPUT}.rom

#riscv32-unknown-elf-objdump -d ${TMPDIR}/a.out
#riscv32-unknown-elf-size ${TMPDIR}/a.out

