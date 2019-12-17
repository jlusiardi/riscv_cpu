#include "../uart.h"
void volatile * const UART_CONFIG = (void *) 0x00000900;
void volatile * const UART_SEND   = (void *) 0x00000902;
void volatile * const MEM = (void *) 0x00001000;


void sleep() {
    register unsigned int i = 1500/22;
    while(i>0) {
        i--;
    }
}

unsigned int str_len (const char *str) {
    return (*str) ? str_len(++str) + 1 : 0;
}

int main() {
    char* text = "Hello Ceara";
    *(unsigned char *)MEM = str_len(text);

    *(unsigned char *)UART_CONFIG = BAUD_MAX;

    for(int i=0;i<str_len(text); i++) {
        *(unsigned char *)UART_SEND   = text[i];
        sleep();
    }

    *(unsigned char *)(MEM + 1) = text[0];
    *(unsigned char *)(MEM + 2) = text[1];

    while(1){}
}
