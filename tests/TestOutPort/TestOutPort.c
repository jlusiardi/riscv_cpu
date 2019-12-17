void volatile * const MEM = (void *) 0x00001000;
void volatile * const OUTPORT = (void *) 0x00000916;

int main() {
    asm("nop");
    *(unsigned char *)OUTPORT   = 'H';
    asm("nop");
    *(unsigned char *)OUTPORT   = 'e';
    asm("nop");
    *(unsigned char *)OUTPORT   = 'l';
    asm("nop");
    *(unsigned char *)OUTPORT   = 'l';
    asm("nop");
    *(unsigned char *)OUTPORT   = 'o';
    asm("nop");
    while(1){}
}
