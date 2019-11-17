all:
	riscv32-unknown-elf-gcc -c -nostdlib -ffreestanding main.c -o main.o
	riscv32-unknown-elf-gcc -c as.s -o as.o
	riscv32-unknown-elf-ld -Tlink.ld main.o as.o -o a.out
	riscv32-unknown-elf-objcopy -O binary a.out a.rom

	riscv32-unknown-elf-objdump -d a.out
	hexdump -C a.rom