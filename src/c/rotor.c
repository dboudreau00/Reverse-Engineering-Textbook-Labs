// rotor.c: an Enigma I simulator (rotors I-II-III, reflector B, rings at A, no plugboard).
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static const char *WIRING[3] = {
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ",   /* rotor I   (left)   */
    "AJDKSIRUXBLHWTMCQGZNPYFVOE",   /* rotor II  (middle) */
    "BDFHJLCPRTXVZNYEIWGAKMUSQO",   /* rotor III (right)  */
};
static const int NOTCH[3] = { 'Q' - 'A', 'E' - 'A', 'V' - 'A' };
static const char *REFLECTOR = "YRUHQSLDPXNGOKMIEBFZCWVJAT";   /* reflector B */
static int pos[3];                                               /* window letters, 0 = A */

static int through(int r, int c) {             /* right-to-left through rotor r */
    return (WIRING[r][(c + pos[r]) % 26] - 'A' - pos[r] + 26) % 26;
}

static int back(int r, int c) {                /* left-to-right through rotor r */
    int i = (int)(strchr(WIRING[r], 'A' + (c + pos[r]) % 26) - WIRING[r]);
    return (i - pos[r] + 26) % 26;
}

static void step(void) {                       /* includes the double-step anomaly */
    if (pos[1] == NOTCH[1]) { pos[1] = (pos[1] + 1) % 26; pos[0] = (pos[0] + 1) % 26; }
    else if (pos[2] == NOTCH[2]) { pos[1] = (pos[1] + 1) % 26; }
    pos[2] = (pos[2] + 1) % 26;
}

static char press(char letter) {
    int c = toupper((unsigned char)letter) - 'A';
    step();
    c = through(2, c); c = through(1, c); c = through(0, c);
    c = REFLECTOR[c] - 'A';
    c = back(0, c); c = back(1, c); c = back(2, c);
    return (char)('A' + c);
}

int main(int argc, char **argv) {
    if (argc < 3 || strlen(argv[1]) != 3) {
        puts("usage: rotor <start positions, e.g. AAA> <message>");
        return 1;
    }
    for (int r = 0; r < 3; r++) pos[r] = toupper((unsigned char)argv[1][r]) - 'A';
    for (const char *p = argv[2]; *p; p++)
        if (isalpha((unsigned char)*p)) putchar(press(*p));
    putchar('\n');
    return 0;
}
