


Disassembly of section .text:

00000000 <_start>:
   0:	00002137          	lui	sp,0x2
   4:	0040006f          	j	8 <main>

00000008 <main>:
   8:	ff010113          	addi	sp,sp,-16 # 1ff0 <OUTPORT+0x1f1c>
   c:	00812623          	sw	s0,12(sp)
  10:	01010413          	addi	s0,sp,16
  14:	00000013          	nop
  18:	000017b7          	lui	a5,0x1
  1c:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x842>
  20:	04800713          	li	a4,72
  24:	00e78023          	sb	a4,0(a5)
  28:	000017b7          	lui	a5,0x1
  2c:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x842>
  30:	02000713          	li	a4,32
  34:	00e78023          	sb	a4,0(a5)
  38:	00000013          	nop
  3c:	000017b7          	lui	a5,0x1
  40:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x842>
  44:	06500713          	li	a4,101
  48:	00e78023          	sb	a4,0(a5)
  4c:	000017b7          	lui	a5,0x1
  50:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x842>
  54:	02000713          	li	a4,32
  58:	00e78023          	sb	a4,0(a5)
  5c:	00000013          	nop
  60:	000017b7          	lui	a5,0x1
  64:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x842>
  68:	06c00713          	li	a4,108
  6c:	00e78023          	sb	a4,0(a5)
  70:	000017b7          	lui	a5,0x1
  74:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x842>
  78:	02000713          	li	a4,32
  7c:	00e78023          	sb	a4,0(a5)
  80:	00000013          	nop
  84:	000017b7          	lui	a5,0x1
  88:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x842>
  8c:	06c00713          	li	a4,108
  90:	00e78023          	sb	a4,0(a5)
  94:	000017b7          	lui	a5,0x1
  98:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x842>
  9c:	02000713          	li	a4,32
  a0:	00e78023          	sb	a4,0(a5)
  a4:	00000013          	nop
  a8:	000017b7          	lui	a5,0x1
  ac:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x842>
  b0:	06f00713          	li	a4,111
  b4:	00e78023          	sb	a4,0(a5)
  b8:	000017b7          	lui	a5,0x1
  bc:	91678793          	addi	a5,a5,-1770 # 916 <OUTPORT+0x842>
  c0:	02e00713          	li	a4,46
  c4:	00e78023          	sb	a4,0(a5)
  c8:	00000013          	nop
  cc:	0000006f          	j	cc <main+0xc4>
