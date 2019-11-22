#!/usr/bin/env bash

. functions.sh

check_for_binary verilator
check_for_binary make

ROM=$1

./compile_rom.sh ${ROM}
verilator -I../main --trace -Wall -Wno-DECLFILENAME -CFLAGS "-std=c++11" --cc cpu.v --exe RunRom.cpp --top-module cpu
make -j 4 -C obj_dir -j -f Vcpu.mk Vcpu
./obj_dir/Vcpu -t -f ${ROM}.rom
