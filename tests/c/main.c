char volatile * const i = (char *) 0x00001000;

int main() {
    *i = 'A';
}
