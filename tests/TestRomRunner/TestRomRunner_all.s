
/tmp/tmp.v6rUzczh93/a.out:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <_start>:
   0:	00002137          	lui	sp,0x2
   4:	0040006f          	j	8 <main>

00000008 <main>:
   8:	ff010113          	addi	sp,sp,-16 # 1ff0 <MEM+0x1f7c>
   c:	00812623          	sw	s0,12(sp)
  10:	01010413          	addi	s0,sp,16
  14:	000017b7          	lui	a5,0x1
  18:	00100713          	li	a4,1
  1c:	00e78023          	sb	a4,0(a5) # 1000 <MEM+0xf8c>
  20:	000017b7          	lui	a5,0x1
  24:	00478793          	addi	a5,a5,4 # 1004 <MEM+0xf90>
  28:	ffe00713          	li	a4,-2
  2c:	00e78023          	sb	a4,0(a5)
  30:	000017b7          	lui	a5,0x1
  34:	00878793          	addi	a5,a5,8 # 1008 <MEM+0xf94>
  38:	00300713          	li	a4,3
  3c:	00e79023          	sh	a4,0(a5)
  40:	000017b7          	lui	a5,0x1
  44:	00c78793          	addi	a5,a5,12 # 100c <MEM+0xf98>
  48:	ffc00713          	li	a4,-4
  4c:	00e79023          	sh	a4,0(a5)
  50:	000017b7          	lui	a5,0x1
  54:	01078793          	addi	a5,a5,16 # 1010 <MEM+0xf9c>
  58:	00500713          	li	a4,5
  5c:	00e7a023          	sw	a4,0(a5)
  60:	000017b7          	lui	a5,0x1
  64:	01478793          	addi	a5,a5,20 # 1014 <MEM+0xfa0>
  68:	ffa00713          	li	a4,-6
  6c:	00e7a023          	sw	a4,0(a5)
  70:	0000006f          	j	70 <main+0x68>
