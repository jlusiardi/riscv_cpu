#!/usr/bin/env bash

. functions.sh

check_for_binary riscv32-unknown-elf-gcc
check_for_binary riscv32-unknown-elf-ld
check_for_binary riscv32-unknown-elf-objcopy
check_for_binary riscv32-unknown-elf-size
check_for_binary riscv32-unknown-elf-objdump
check_for_binary srec_cat


TMPDIR=`mktemp -d`
INPUT="$1"
# use this to enable compressed extension
#CC_OPTS="-Wa,-march=rv32ic"

cat <<EOT >> ${TMPDIR}/link.ld
OUTPUT_FORMAT(elf32-littleriscv)

ENTRY(_start);

MEMORY {
  rom (rx) : ORIGIN = 0x00000000, LENGTH = 2048
  ram (wx) : ORIGIN = 0x00001000, LENGTH = 8912
}

SECTIONS {
    .text : {
            *(.text.prologue);
            *(.text*);
        } > rom
}
EOT

riscv32-unknown-elf-gcc ${CC_OPTS} -c -nostdlib -ffreestanding ${INPUT}/${INPUT}.c -o ${TMPDIR}/main.o
riscv32-unknown-elf-gcc ${CC_OPTS} -c ${INPUT}/${INPUT}.s -o ${TMPDIR}/as.o 
riscv32-unknown-elf-ld -T${TMPDIR}/link.ld ${TMPDIR}/*.o -o ${TMPDIR}/a.out
riscv32-unknown-elf-objcopy -O binary ${TMPDIR}/a.out ${INPUT}/${INPUT}.rom
srec_cat ${INPUT}/${INPUT}.rom -binary -output ${INPUT}/${INPUT}.mif -mif -enable=header -HEAder "MIF for >${INPUT}<"

riscv32-unknown-elf-objdump -d ${TMPDIR}/a.out | grep -v ${TMPDIR} > ${INPUT}/${INPUT}_all.s
#riscv32-unknown-elf-size ${TMPDIR}/a.out

