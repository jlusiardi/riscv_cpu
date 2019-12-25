void volatile * const MEM = (void *) 0x00001000;
void volatile * const OUTPORT = (void *) 0x00000916;

int main() {
    asm("nop");
    *(unsigned char *)OUTPORT   = 'H';
    *(unsigned char *)OUTPORT   = ' ';
    asm("nop");
    *(unsigned char *)OUTPORT   = 'e';
    *(unsigned char *)OUTPORT   = ' ';
    asm("nop");
    *(unsigned char *)OUTPORT   = 'l';
    *(unsigned char *)OUTPORT   = ' ';
    asm("nop");
    *(unsigned char *)OUTPORT   = 'l';
    *(unsigned char *)OUTPORT   = ' ';
    asm("nop");
    *(unsigned char *)OUTPORT   = 'o';
    *(unsigned char *)OUTPORT   = '.';
    asm("nop");
    while(1){}
}
