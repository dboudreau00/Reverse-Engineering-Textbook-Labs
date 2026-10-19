#include <stdio.h>
#include <string.h>

static const unsigned char enc[] = { 0x3F, 0x29, 0x28, 0x3F, 0x2C, 0x3F, 0x28, 0x7B };

int check(const char *s) {
    if (strlen(s) != sizeof enc) return 0;
    for (size_t i = 0; i < sizeof enc; i++)
        if ((unsigned char)(s[i] ^ 0x5A) != enc[i]) return 0;
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1 && check(argv[1])) {
        puts("Correct!");
        return 0;
    }
    puts("Wrong.");
    return 1;
}
