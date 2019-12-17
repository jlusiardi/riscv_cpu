
/tmp/tmp.yM0TTaGwTT/a.out:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <_start>:
   0:	00002137          	lui	sp,0x2
   4:	0940006f          	j	98 <main>

00000008 <sleep>:
   8:	ff010113          	addi	sp,sp,-16 # 1ff0 <MEM+0x1e98>
   c:	00812623          	sw	s0,12(sp)
  10:	00912423          	sw	s1,8(sp)
  14:	01010413          	addi	s0,sp,16
  18:	04400493          	li	s1,68
  1c:	0080006f          	j	24 <sleep+0x1c>
  20:	fff48493          	addi	s1,s1,-1
  24:	fe049ee3          	bnez	s1,20 <sleep+0x18>
  28:	00000013          	nop
  2c:	00000013          	nop
  30:	00c12403          	lw	s0,12(sp)
  34:	00812483          	lw	s1,8(sp)
  38:	01010113          	addi	sp,sp,16
  3c:	00008067          	ret

00000040 <str_len>:
  40:	fe010113          	addi	sp,sp,-32
  44:	00112e23          	sw	ra,28(sp)
  48:	00812c23          	sw	s0,24(sp)
  4c:	02010413          	addi	s0,sp,32
  50:	fea42623          	sw	a0,-20(s0)
  54:	fec42783          	lw	a5,-20(s0)
  58:	0007c783          	lbu	a5,0(a5)
  5c:	02078263          	beqz	a5,80 <str_len+0x40>
  60:	fec42783          	lw	a5,-20(s0)
  64:	00178793          	addi	a5,a5,1
  68:	fef42623          	sw	a5,-20(s0)
  6c:	fec42503          	lw	a0,-20(s0)
  70:	fd1ff0ef          	jal	ra,40 <str_len>
  74:	00050793          	mv	a5,a0
  78:	00178793          	addi	a5,a5,1
  7c:	0080006f          	j	84 <str_len+0x44>
  80:	00000793          	li	a5,0
  84:	00078513          	mv	a0,a5
  88:	01c12083          	lw	ra,28(sp)
  8c:	01812403          	lw	s0,24(sp)
  90:	02010113          	addi	sp,sp,32
  94:	00008067          	ret

00000098 <main>:
  98:	fe010113          	addi	sp,sp,-32
  9c:	00112e23          	sw	ra,28(sp)
  a0:	00812c23          	sw	s0,24(sp)
  a4:	02010413          	addi	s0,sp,32
  a8:	15c00793          	li	a5,348
  ac:	fef42423          	sw	a5,-24(s0)
  b0:	fe842503          	lw	a0,-24(s0)
  b4:	f8dff0ef          	jal	ra,40 <str_len>
  b8:	00050713          	mv	a4,a0
  bc:	000017b7          	lui	a5,0x1
  c0:	0ff77713          	andi	a4,a4,255
  c4:	00e78023          	sb	a4,0(a5) # 1000 <MEM+0xea8>
  c8:	000017b7          	lui	a5,0x1
  cc:	90078793          	addi	a5,a5,-1792 # 900 <MEM+0x7a8>
  d0:	00f00713          	li	a4,15
  d4:	00e78023          	sb	a4,0(a5)
  d8:	fe042623          	sw	zero,-20(s0)
  dc:	0300006f          	j	10c <main+0x74>
  e0:	fec42783          	lw	a5,-20(s0)
  e4:	fe842703          	lw	a4,-24(s0)
  e8:	00f70733          	add	a4,a4,a5
  ec:	000017b7          	lui	a5,0x1
  f0:	90278793          	addi	a5,a5,-1790 # 902 <MEM+0x7aa>
  f4:	00074703          	lbu	a4,0(a4)
  f8:	00e78023          	sb	a4,0(a5)
  fc:	f0dff0ef          	jal	ra,8 <sleep>
 100:	fec42783          	lw	a5,-20(s0)
 104:	00178793          	addi	a5,a5,1
 108:	fef42623          	sw	a5,-20(s0)
 10c:	fe842503          	lw	a0,-24(s0)
 110:	f31ff0ef          	jal	ra,40 <str_len>
 114:	00050713          	mv	a4,a0
 118:	fec42783          	lw	a5,-20(s0)
 11c:	fce7e2e3          	bltu	a5,a4,e0 <main+0x48>
 120:	000017b7          	lui	a5,0x1
 124:	00178793          	addi	a5,a5,1 # 1001 <MEM+0xea9>
 128:	fe842703          	lw	a4,-24(s0)
 12c:	00074703          	lbu	a4,0(a4)
 130:	00e78023          	sb	a4,0(a5)
 134:	fe842783          	lw	a5,-24(s0)
 138:	00178713          	addi	a4,a5,1
 13c:	000017b7          	lui	a5,0x1
 140:	00278793          	addi	a5,a5,2 # 1002 <MEM+0xeaa>
 144:	00074703          	lbu	a4,0(a4)
 148:	00e78023          	sb	a4,0(a5)
 14c:	0000006f          	j	14c <main+0xb4>
