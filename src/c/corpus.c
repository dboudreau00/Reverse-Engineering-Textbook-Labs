// corpus.c: twelve small, well-known routines. Each is kept as its own function.
#include <stddef.h>
#include <stdint.h>
#define KEEP static __attribute__((noinline, used))

KEEP void xor_decode(uint8_t *buf, size_t n, uint8_t key) {
    for (size_t i = 0; i < n; i++) buf[i] ^= key;
}

KEEP void rc4_ksa(uint8_t S[256], const uint8_t *key, size_t klen) {
    for (int i = 0; i < 256; i++) S[i] = (uint8_t)i;
    for (int i = 0, j = 0; i < 256; i++) {
        j = (j + S[i] + key[i % klen]) & 0xFF;
        uint8_t t = S[i]; S[i] = S[j]; S[j] = t;
    }
}

KEEP void rc4_prga(uint8_t S[256], uint8_t *buf, size_t n) {
    int i = 0, j = 0;
    for (size_t k = 0; k < n; k++) {
        i = (i + 1) & 0xFF;
        j = (j + S[i]) & 0xFF;
        uint8_t t = S[i]; S[i] = S[j]; S[j] = t;
        buf[k] ^= S[(S[i] + S[j]) & 0xFF];
    }
}

KEEP uint32_t fnv1a32(const uint8_t *p, size_t n) {
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < n; i++) { h ^= p[i]; h *= 16777619u; }
    return h;
}

KEEP uint32_t crc32_bitwise(const uint8_t *p, size_t n) {
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < n; i++) {
        crc ^= p[i];
        for (int b = 0; b < 8; b++) crc = (crc >> 1) ^ (0xEDB88320u & (0u - (crc & 1)));
    }
    return ~crc;
}

KEEP size_t base64_encode(const uint8_t *in, size_t n, char *out) {
    static const char T[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t o = 0;
    for (size_t i = 0; i < n; i += 3) {
        uint32_t v = (uint32_t)in[i] << 16 | (i + 1 < n ? in[i + 1] << 8 : 0) | (i + 2 < n ? in[i + 2] : 0);
        out[o++] = T[v >> 18 & 63];
        out[o++] = T[v >> 12 & 63];
        out[o++] = i + 1 < n ? T[v >> 6 & 63] : '=';
        out[o++] = i + 2 < n ? T[v & 63] : '=';
    }
    out[o] = 0;
    return o;
}

KEEP void bubble_sort(int *a, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (a[j] > a[j + 1]) { int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t; }
}

KEEP size_t str_len(const char *s) {
    const char *p = s;
    while (*p) p++;
    return (size_t)(p - s);
}

KEEP void reverse_string(char *s, size_t n) {
    for (size_t i = 0, j = n ? n - 1 : 0; i < j; i++, j--) { char t = s[i]; s[i] = s[j]; s[j] = t; }
}

KEEP int popcount32(uint32_t v) {
    int c = 0;
    while (v) { v &= v - 1; c++; }
    return c;
}

KEEP uint32_t gcd(uint32_t a, uint32_t b) {
    while (b) { uint32_t t = a % b; a = b; b = t; }
    return a;
}

KEEP int is_prime(uint32_t n) {
    if (n < 2) return 0;
    for (uint32_t d = 2; (uint64_t)d * d <= n; d++) if (n % d == 0) return 0;
    return 1;
}
