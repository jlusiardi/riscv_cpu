#!/usr/bin/env bash

. functions.sh

check_for_binary verilator
check_for_binary make

ROM=$1

./compile_rom.sh ${ROM}
verilator -I../main --trace -Wall -Wno-DECLFILENAME -CFLAGS "-std=c++11" --cc cpu_synth.v --exe RunRomSynth.cpp --top-module cpu_synth
make -j 4 -C obj_dir -j -f Vcpu_synth.mk Vcpu_synth
./obj_dir/Vcpu_synth -t -f ${ROM}.rom -T ${ROM}.tests
