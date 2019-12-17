void volatile * const MEM = (void *) 0x00001000;

unsigned int fib(unsigned int in) {
    if (in < 3) {
        return 1;
    } else {
        return fib(in - 1) + fib(in - 2);
    }
}

int main() {
    *(unsigned int *)MEM = fib(3);
    *(unsigned int *)(MEM+4) = fib(4);
    *(unsigned int *)(MEM+8) = fib(13);
    *(unsigned int *)(MEM+12) = fib(14);
    while(1){}
}
