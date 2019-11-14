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
    make -j 4 -C obj_dir -j -f V${MODULE}.mk V${MODULE}
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
    echo -e "\e[1m ---> verilate $MODULE <---\e[0m"
    verilate_module $MODULE
    if [ $? != 0 ]; then
        echo -e "\e[31mFailed to verilate $MODULE!\e[39m"
        exit 1
    fi
    echo -e "\e[1m ---> compile $MODULE <---\e[0m"
    make_module $MODULE
    if [ $? != 0 ]; then
        echo -e "\e[31mFailed to compile $MODULE!\e[39m"
        exit 1
    fi
done

for MODULE in $MODULES
do
    echo -e "\e[1m ---> run $MODULE <---\e[0m"
    run_test $MODULE
done
