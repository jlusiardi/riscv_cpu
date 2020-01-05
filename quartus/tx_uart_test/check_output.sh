#!/usr/bin/env bash

. ../../tests/functions.sh

TMPFILE=`mktemp`
function cleanup()
{
	echo "cleaning up $TMPFILE"
    rm $TMPFILE
}

trap cleanup EXIT

set_green_foreground
echo "#########################################################"
echo "# Testing!                                              #"
echo "#########################################################"
set_normal_foreground

LA_DEV=`sigrok-cli -d fx2lafw --scan | grep -v "The following" | sed 's/ - .*//'`
sigrok-cli -d ${LA_DEV} \
        --config samplerate="16 MHz" \
        -P uart:rx=D7:format=ascii \
        --samples 50000 \
    | grep -B 1 "Stop bit" \
    | grep -v "Stop bit" \
    | grep -v -- "--" \
    | grep -m 1 -A 15 "0D" > $TMPFILE

diff -y --color=always $TMPFILE expected_result.txt
