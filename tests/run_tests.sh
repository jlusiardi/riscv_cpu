#!/usr/bin/env bash

function verilate_module() {
    local MODULE=$1
    echo "verilate ${MODULE}"
    verilator -I../main --trace -Wall -Wno-DECLFILENAME -CFLAGS "-std=c++11" \
        --cc ${MODULE}.v --exe ${MODULE}_main.cpp --top-module ${MODULE} -DDEBUG_ON
}

function make_module() {
    local MODULE=$1
    echo "make ${MODULE}"
    make -C obj_dir -j -f V${MODULE}.mk V${MODULE}
}

function run_test() {
    local MODULE=$1
    ./obj_dir/V${MODULE}
}

if [ -z "$@" ]; then
    MODULES="alu memory register_file ram_memory rom_memory decoder memory_control compare_unit cpu"
else
    MODULES="$@"
fi

for MODULE in $MODULES
do
    verilate_module $MODULE
    make_module $MODULE
done

for MODULE in $MODULES
do
    run_test $MODULE
done
