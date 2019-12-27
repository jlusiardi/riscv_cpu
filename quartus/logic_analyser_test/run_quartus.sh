#!/usr/bin/env bash

. ../../tests/functions.sh

check_for_binary quartus_map
check_for_binary quartus_fit
check_for_binary quartus_asm
check_for_binary quartus_sta
check_for_binary quartus_pgm

quartus_map --read_settings_files=on --write_settings_files=off logic_analyser_test

quartus_fit --read_settings_files=off --write_settings_files=off logic_analyser_test

quartus_asm --read_settings_files=off --write_settings_files=off logic_analyser_test

quartus_sta logic_analyser_test

CABLE_NAME=`quartus_pgm -l | grep -v "Info" | sed -E 's/[0-9]+\) //'`

cat << EOF > output_files/logic_analyser_test.cdf
/* Quartus II 64-Bit Version 13.0.1 Build 232 06/12/2013 Service Pack 1 SJ Web Edition */
JedecChain;
	FileRevision(JESD32A);
	DefaultMfr(6E);

	P ActionCode(Cfg)
		Device PartName(EP2C5T144) Path("/home/jlusiardi/Dokumente/src/riscv_cpu/quartus/logic_analyser_test/output_files/") File("logic_analyser_test.sof") MfrSpec(OpMask(1));

ChainEnd;

AlteraBegin;
	ChainType(JTAG);
AlteraEnd;
EOF

quartus_pgm -c "${CABLE_NAME}" output_files/logic_analyser_test.cdf
