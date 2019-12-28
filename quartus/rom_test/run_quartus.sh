#!/usr/bin/env bash

set -e

. ../../tests/functions.sh

check_for_binary quartus_map
check_for_binary quartus_fit
check_for_binary quartus_asm
check_for_binary quartus_sta
check_for_binary quartus_pgm
check_for_binary sigrok-cli

quartus_map --read_settings_files=on --write_settings_files=off main

quartus_fit --read_settings_files=off --write_settings_files=off main

quartus_asm --read_settings_files=off --write_settings_files=off main

quartus_sta main

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

quartus_pgm -c "${CABLE_NAME}" output_files/main.cdf


LA_DEV=`sigrok-cli -d fx2lafw --scan | grep -v "The following" | sed 's/ - .*//'`
sigrok-cli -d ${LA_DEV} --config samplerate="16 MHz" -P parallel:d0=D0:d1=D1:d2=D2:d3=D3:d4=D4:d5=D5:d6=D5  --samples 25