#!/usr/bin/env bash

. ../tests/functions.sh

check_for_binary quartus_map
check_for_binary quartus_fit
check_for_binary quartus_asm
check_for_binary quartus_sta
check_for_binary quartus_pgm

quartus_map --read_settings_files=on --write_settings_files=off riscv_cpu -c riscv_cpu

quartus_fit --read_settings_files=off --write_settings_files=off riscv_cpu -c riscv_cpu

quartus_asm --read_settings_files=off --write_settings_files=off riscv_cpu -c riscv_cpu

quartus_sta riscv_cpu -c riscv_cpu

CABLE_NAME=`quartus_pgm -l | grep -v "Info" | sed 's/.*) //'`
quartus_pgm -c "${CABLE_NAME}" output_files/riscv_cpu.cdf