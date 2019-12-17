void volatile * const MEM = (void *) 0x00001000;

int main() {
    *(unsigned char *)MEM = 1;
    *(signed char *)(MEM+4) = -2;
    *(unsigned short *)(MEM+8) = 3;
    *(signed short *)(MEM+12) = -4;
    *(unsigned int *)(MEM+16) = 5;
    *(signed int *)(MEM+20) = -6;
    while(1){}
}
