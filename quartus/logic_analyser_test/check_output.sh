#!/usr/bin/env bash

TMPFILE=`mktemp`
function cleanup()
{
	echo "cleaning up $TMPFILE"
    rm $TMPFILE
}

trap cleanup EXIT

echo "#########################################################"
echo "# Testing!                                              #"
echo "#########################################################"

LA_DEV=`sigrok-cli -d fx2lafw --scan | grep -v "The following" | sed 's/ - .*//'`
sigrok-cli -d ${LA_DEV} \
        --config samplerate="16 MHz" \
	    -P parallel:d0=D0:d1=D1:d2=D2:d3=D3:d4=D4:d5=D5:d6=D6:d7=D7 \
	    --samples 2000 \
    | grep -m 1 -A 15 "parallel-1: 00" > $TMPFILE

diff -y --color=always $TMPFILE expected_result.txt
