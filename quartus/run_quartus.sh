#!/usr/bin/env bash

DIRECTORY=$1
shift
LOGLEVEL=$1
shift

cd $DIRECTORY

TMPFILE=`mktemp`
function cleanup()
{
	echo "cleaning up $TMPFILE"
    rm $TMPFILE
}

trap cleanup EXIT

. ../../tests/functions.sh

check_for_binary quartus_map
check_for_binary quartus_fit
check_for_binary quartus_asm
check_for_binary quartus_sta
check_for_binary quartus_pgm
check_for_binary sigrok-cli

if [ "$LOGLEVEL" = "LESS" ]; then
	PATTERN="Warning|Error"
else
	PATTERN=".*"
fi

set_green_foreground
echo "#########################################################"
echo "# Mapping!                                              #"
echo "#########################################################"
set_normal_foreground
quartus_map --read_settings_files=on --write_settings_files=off main > $TMPFILE
RESULT=$?
cat $TMPFILE | grep -E "$PATTERN" || true
if [ $RESULT -ne 0 ]; then
	exit $RESULT
fi
show_file output_files/main.map.summary

set_green_foreground
echo "#########################################################"
echo "# Fitting!                                              #"
echo "#########################################################"
set_normal_foreground
quartus_fit --read_settings_files=off --write_settings_files=off main > $TMPFILE
RESULT=$?
cat $TMPFILE | grep -E "$PATTERN" || true
if [ $RESULT -ne 0 ]; then
	exit $RESULT
fi
show_file output_files/main.fit.summary

set_green_foreground
echo "#########################################################"
echo "# Assembling!                                           #"
echo "#########################################################"
set_normal_foreground
quartus_asm --read_settings_files=off --write_settings_files=off main > $TMPFILE
RESULT=$?
cat $TMPFILE | grep -E "$PATTERN" || true
if [ $RESULT -ne 0 ]; then
	exit $RESULT
fi

set_green_foreground
echo "#########################################################"
echo "# Analysing timing                                      #"
echo "#########################################################"
set_normal_foreground
quartus_sta main > $TMPFILE
RESULT=$?
cat $TMPFILE | grep -E "$PATTERN" || true
if [ $RESULT -ne 0 ]; then
	exit $RESULT
fi
show_file output_files/main.sta.summary

CABLE_NAME=`quartus_pgm -l | grep -v "Info" | sed -E 's/[0-9]+\) //'`


cat << EOF > output_files/main.cdf
/* Quartus II 64-Bit Version 13.0.1 Build 232 06/12/2013 Service Pack 1 SJ Web Edition */
JedecChain;
	FileRevision(JESD32A);
	DefaultMfr(6E);

	P ActionCode(Cfg)
		Device PartName(EP2C5T144) Path("$PWD/output_files/") File("main.sof") MfrSpec(OpMask(1));

ChainEnd;

AlteraBegin;
	ChainType(JTAG);
AlteraEnd;
EOF

set_green_foreground
echo "#########################################################"
echo "# Flashing!                                             #"
echo "#########################################################"
set_normal_foreground
quartus_pgm -c "${CABLE_NAME}" output_files/main.cdf > $TMPFILE
RESULT=$?
cat $TMPFILE | grep -E "$PATTERN" || true
if [ $RESULT -ne 0 ]; then
	exit $RESULT
fi

if [[ -x "check_output.sh" ]]; then
	./check_output.sh
fi