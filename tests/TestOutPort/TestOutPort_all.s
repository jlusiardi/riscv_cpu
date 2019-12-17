


Disassembly of section .text:

00000000 <_start>:
   0:	00002137          	lui	sp,0x2
   4:	0040006f          	j	8 <main>

00000008 <main>:
   8:	ff010113          	addi	sp,sp,-16 # 1ff0 <OUTPORT+0x1f6c>
   c:	00812623          	sw	s0,12(sp)
  10:	01010413          	addi	s0,sp,16
  14:	00000013          	nop
  18:	000017b7          	lui	a5,0x1
  1c:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x892>
  20:	04800713          	li	a4,72
  24:	00e78023          	sb	a4,0(a5)
  28:	00000013          	nop
  2c:	000017b7          	lui	a5,0x1
  30:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x892>
  34:	06500713          	li	a4,101
  38:	00e78023          	sb	a4,0(a5)
  3c:	00000013          	nop
  40:	000017b7          	lui	a5,0x1
  44:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x892>
  48:	06c00713          	li	a4,108
  4c:	00e78023          	sb	a4,0(a5)
  50:	00000013          	nop
  54:	000017b7          	lui	a5,0x1
  58:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x892>
  5c:	06c00713          	li	a4,108
  60:	00e78023          	sb	a4,0(a5)
  64:	00000013          	nop
  68:	000017b7          	lui	a5,0x1
  6c:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x892>
  70:	06f00713          	li	a4,111
  74:	00e78023          	sb	a4,0(a5)
  78:	00000013          	nop
  7c:	0000006f          	j	7c <main+0x74>
