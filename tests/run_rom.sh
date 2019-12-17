#!/usr/bin/env bash

set -e

. functions.sh

check_for_binary verilator
check_for_binary make

ROM=$1

./compile_rom.sh ${ROM}
verilator -I../main --trace -Wall -Wno-DECLFILENAME -CFLAGS "-std=c++11" --cc soc.v --exe RunRom.cpp --top-module soc
make -j 4 -C obj_dir -j -f Vsoc.mk Vsoc
time ./obj_dir/Vsoc -t -f ${ROM}/${ROM}.rom -T ${ROM}/${ROM}.tests
