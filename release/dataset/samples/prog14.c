#include "corpus.c"
#include <stdio.h>
int main(int argc, char **argv) {
    uint8_t buf[64] = {0}, S[256]; char out[128]; int nums[16]; uint32_t acc = (uint32_t)argc;
    for (int i = 0; i < 16; i++) nums[i] = (int)(acc * 2654435761u >> (i % 24));
    for (int i = 0; i < 64; i++) buf[i] = (uint8_t)(argv[0][i % 8] + i);
    acc ^= (uint32_t)popcount32(acc);
    rc4_prga(S, buf, sizeof buf);
    acc ^= (uint32_t)base64_encode(buf, 48, out);
    acc ^= crc32_bitwise(buf, sizeof buf);
    xor_decode(buf, sizeof buf, 0x5A);
    acc ^= (uint32_t)str_len(out);
    printf("%u\n", acc);
    return 0;
}
