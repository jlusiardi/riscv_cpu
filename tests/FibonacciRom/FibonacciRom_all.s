
/tmp/tmp.45EcC3bzxd/a.out:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <_start>:
   0:	00002137          	lui	sp,0x2
   4:	0740006f          	j	78 <main>

00000008 <fib>:
   8:	fe010113          	addi	sp,sp,-32 # 1fe0 <MEM+0x1ef4>
   c:	00112e23          	sw	ra,28(sp)
  10:	00812c23          	sw	s0,24(sp)
  14:	00912a23          	sw	s1,20(sp)
  18:	02010413          	addi	s0,sp,32
  1c:	fea42623          	sw	a0,-20(s0)
  20:	fec42703          	lw	a4,-20(s0)
  24:	00200793          	li	a5,2
  28:	00e7e663          	bltu	a5,a4,34 <fib+0x2c>
  2c:	00100793          	li	a5,1
  30:	0300006f          	j	60 <fib+0x58>
  34:	fec42783          	lw	a5,-20(s0)
  38:	fff78793          	addi	a5,a5,-1
  3c:	00078513          	mv	a0,a5
  40:	fc9ff0ef          	jal	ra,8 <fib>
  44:	00050493          	mv	s1,a0
  48:	fec42783          	lw	a5,-20(s0)
  4c:	ffe78793          	addi	a5,a5,-2
  50:	00078513          	mv	a0,a5
  54:	fb5ff0ef          	jal	ra,8 <fib>
  58:	00050793          	mv	a5,a0
  5c:	00f487b3          	add	a5,s1,a5
  60:	00078513          	mv	a0,a5
  64:	01c12083          	lw	ra,28(sp)
  68:	01812403          	lw	s0,24(sp)
  6c:	01412483          	lw	s1,20(sp)
  70:	02010113          	addi	sp,sp,32
  74:	00008067          	ret

00000078 <main>:
  78:	ff010113          	addi	sp,sp,-16
  7c:	00112623          	sw	ra,12(sp)
  80:	00812423          	sw	s0,8(sp)
  84:	00912223          	sw	s1,4(sp)
  88:	01010413          	addi	s0,sp,16
  8c:	000014b7          	lui	s1,0x1
  90:	00300513          	li	a0,3
  94:	f75ff0ef          	jal	ra,8 <fib>
  98:	00050793          	mv	a5,a0
  9c:	00f4a023          	sw	a5,0(s1) # 1000 <MEM+0xf14>
  a0:	000017b7          	lui	a5,0x1
  a4:	00478493          	addi	s1,a5,4 # 1004 <MEM+0xf18>
  a8:	00400513          	li	a0,4
  ac:	f5dff0ef          	jal	ra,8 <fib>
  b0:	00050793          	mv	a5,a0
  b4:	00f4a023          	sw	a5,0(s1)
  b8:	000017b7          	lui	a5,0x1
  bc:	00878493          	addi	s1,a5,8 # 1008 <MEM+0xf1c>
  c0:	00d00513          	li	a0,13
  c4:	f45ff0ef          	jal	ra,8 <fib>
  c8:	00050793          	mv	a5,a0
  cc:	00f4a023          	sw	a5,0(s1)
  d0:	000017b7          	lui	a5,0x1
  d4:	00c78493          	addi	s1,a5,12 # 100c <MEM+0xf20>
  d8:	00e00513          	li	a0,14
  dc:	f2dff0ef          	jal	ra,8 <fib>
  e0:	00050793          	mv	a5,a0
  e4:	00f4a023          	sw	a5,0(s1)
  e8:	0000006f          	j	e8 <main+0x70>
