#!/usr/bin/env bash

. functions.sh

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

check_for_binary verilator
check_for_binary make
check_for_binary g++
check_for_binary ar

if [ $# -ge 1 ]; then
    MODULES="$@"
else
    MODULES="alu memory register_file decoder compare_unit memory_control memory ram_memory rom_memory cpu"
fi

for MODULE in $MODULES
do
    echo -e "\e[92m ---> verilate $MODULE <---\e[0m"
    verilate_module $MODULE
    if [ $? != 0 ]; then
        echo -e "\e[31mFailed to verilate $MODULE!\e[39m"
        exit 1
    fi
    echo -e "\e[92m ---> compile $MODULE <---\e[0m"
    make_module $MODULE
    if [ $? != 0 ]; then
        echo -e "\e[31mFailed to compile $MODULE!\e[39m"
        exit 1
    fi
done

ERROR=0
for MODULE in $MODULES
do
    echo -e "\e[92m ---> run $MODULE <---\e[0m"
    run_test $MODULE
    STATUS=$?
    if [ $STATUS != 0 ]; then
        ((ERROR=ERROR+STATUS))
        echo -e "\e[31m${STATUS} Failed tests in $MODULE!\e[39m"
    fi
done
if [ $ERROR != 0 ]; then
    echo -e "\e[31mThere were ${ERROR} failed tests!\e[39m"
    exit 1
fi
