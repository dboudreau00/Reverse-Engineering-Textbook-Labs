// dga.c: a toy domain generation algorithm (DGA) for Lab 18.1.
// It prints names under .test, a top-level domain reserved for testing, and contacts nothing.
#include <stdio.h>
#include <windows.h>

#define PER_DAY 4
#define LENGTH 12

static void domains_for(unsigned year, unsigned month, unsigned day) {
    unsigned seed = (year * 10000 + month * 100 + day) ^ 0x5EED5EED;
    for (int n = 0; n < PER_DAY; n++) {
        char name[LENGTH + 1];
        for (int i = 0; i < LENGTH; i++) {
            seed = seed * 1103515245u + 12345u;
            name[i] = 'a' + (seed >> 16) % 26;
        }
        name[LENGTH] = '\0';
        printf("%s.example.test\n", name);
    }
}

int main(int argc, char **argv) {
    SYSTEMTIME now;
    GetSystemTime(&now);                                   /* UTC, as many real DGAs use */
    unsigned year = now.wYear, month = now.wMonth, day = now.wDay;
    if (argc == 2 && sscanf(argv[1], "%u-%u-%u", &year, &month, &day) != 3)   /* lab only: pick a date */
        return 1;
    domains_for(year, month, day);
    return 0;
}
