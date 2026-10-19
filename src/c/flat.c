// flat.c: Lab 5's check, written plainly and again in control-flow-flattened form.
#include <stdio.h>
#include <string.h>

static const unsigned char enc[] = { 0x3F, 0x29, 0x28, 0x3F, 0x2C, 0x3F, 0x28, 0x7B };

__attribute__((noinline)) int check(const char *s) {
    if (strlen(s) != sizeof enc) return 0;
    for (size_t i = 0; i < sizeof enc; i++)
        if ((unsigned char)(s[i] ^ 0x5A) != enc[i]) return 0;
    return 1;
}

/* The same logic as a dispatcher loop. Every block sets the next state and jumps back to the switch.
   'volatile' stops the compiler from undoing the flattening, as real obfuscators arrange. */
__attribute__((noinline)) int check_flat(const char *s) {
    volatile int state = 0;
    size_t i = 0;
    int result = 0;
    for (;;) {
        switch (state) {
        case 0: state = strlen(s) == sizeof enc ? 1 : 4; break;
        case 1: state = i < sizeof enc ? 2 : 5; break;
        case 2: state = (unsigned char)(s[i] ^ 0x5A) == enc[i] ? 3 : 4; break;
        case 3: i++; state = 1; break;
        case 4: result = 0; state = 6; break;
        case 5: result = 1; state = 6; break;
        case 6: return result;
        }
    }
}

int main(int argc, char **argv) {
    if (argc > 1 && check(argv[1]) && check_flat(argv[1])) {
        puts("Correct!");
        return 0;
    }
    puts("Wrong.");
    return 1;
}
