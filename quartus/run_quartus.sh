#!/usr/bin/env bash

cd $1

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

PATTERN="Warning|Error"
PATTERN=".*"

echo "#########################################################"
echo "# Mapping!                                              #"
echo "#########################################################"
quartus_map --read_settings_files=on --write_settings_files=off main > $TMPFILE
RESULT=$?
cat /tmp/foo | grep -E "$PATTERN" || true
echo $RESULT
if [ $RESULT -ne 0 ]; then
	exit $RESULT
fi

echo "#########################################################"
echo "# Fitting!                                              #"
echo "#########################################################"
quartus_fit --read_settings_files=off --write_settings_files=off main | grep -E "$PATTERN" || true

echo "#########################################################"
echo "# Assembling!                                           #"
echo "#########################################################"
quartus_asm --read_settings_files=off --write_settings_files=off main | grep -E "$PATTERN" || true

echo "#########################################################"
echo "# Analysing timing                                      #"
echo "#########################################################"
quartus_sta main | grep -E "$PATTERN" || true

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

echo "#########################################################"
echo "# Flashing!                                             #"
echo "#########################################################"
quartus_pgm -c "${CABLE_NAME}" output_files/main.cdf | grep "$PATTERN" || true

if [[ -x "check_output.sh" ]]; then
	./check_output.sh
fi