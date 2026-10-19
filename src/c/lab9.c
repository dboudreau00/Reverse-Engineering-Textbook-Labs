// lab9.c: a harmless "beacon" that only prints its own configuration.
#include <stdio.h>
#include <string.h>
#include <windows.h>

static void rc4(const unsigned char *key, size_t klen, unsigned char *buf, size_t n) {
    unsigned char S[256], t;
    unsigned i, j = 0;
    for (i = 0; i < 256; i++) S[i] = (unsigned char)i;
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % klen]) & 0xFF;
        t = S[i]; S[i] = S[j]; S[j] = t;
    }
    i = j = 0;
    for (size_t k = 0; k < n; k++) {
        i = (i + 1) & 0xFF;
        j = (j + S[i]) & 0xFF;
        t = S[i]; S[i] = S[j]; S[j] = t;
        buf[k] ^= S[(S[i] + S[j]) & 0xFF];
    }
}

int main(void) {
    HRSRC res = FindResourceA(NULL, "CONFIG", RT_RCDATA);
    DWORD size = SizeofResource(NULL, res);
    const unsigned char *blob = LockResource(LoadResource(NULL, res));
    unsigned char klen = blob[0];
    char cfg[512] = {0};
    memcpy(cfg, blob + 1 + klen, size - 1 - klen);
    rc4(blob + 1, klen, (unsigned char *)cfg, size - 1 - klen);
    puts(cfg);
    return 0;
}
