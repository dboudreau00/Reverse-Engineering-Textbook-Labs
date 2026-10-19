// lab9c.c: strings polluted with a junk marker and cleaned at run time.
#include <stdio.h>
#include <string.h>

#define MARKER "q9zk!"
static const char *POLLUTED[] = {
    "https://c2.exa" MARKER "mple.test/gate",
    "Mozilla/5.0 (esre" MARKER "veR lab)",
    "HKCU\\Software\\esre" MARKER "veR",
    "Glo" MARKER "bal\\LabMtx",
    "set" MARKER "tings.ini",
    "uplo" MARKER "ad.php",
    "screen" MARKER "shot.png",
    "brow" MARKER "ser_history.txt",
    "wal" MARKER "lets.txt",
    "sys" MARKER "info.txt",
    "mach" MARKER "ine_id",
    "sle" MARKER "ep=60",
};

static void clean(const char *in, char *out) {      /* copy, skipping every marker occurrence */
    size_t m = strlen(MARKER);
    while (*in) {
        if (strncmp(in, MARKER, m) == 0) in += m;
        else *out++ = *in++;
    }
    *out = 0;
}

int main(void) {
    char buf[128];
    for (size_t i = 0; i < sizeof POLLUTED / sizeof *POLLUTED; i++) {
        clean(POLLUTED[i], buf);
        puts(buf);
    }
    return 0;
}
