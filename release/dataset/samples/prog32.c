#include "corpus.c"
#include <stdio.h>
int main(int argc, char **argv) {
    uint8_t buf[64] = {0}, S[256]; char out[128]; int nums[16]; uint32_t acc = (uint32_t)argc;
    for (int i = 0; i < 16; i++) nums[i] = (int)(acc * 2654435761u >> (i % 24));
    for (int i = 0; i < 64; i++) buf[i] = (uint8_t)(argv[0][i % 8] + i);
    acc ^= fnv1a32(buf, sizeof buf);
    rc4_prga(S, buf, sizeof buf);
    xor_decode(buf, sizeof buf, 0x5A);
    reverse_string(out, 16);
    acc ^= (uint32_t)is_prime(acc);
    acc ^= (uint32_t)str_len(out);
    bubble_sort(nums, 16);
    printf("%u\n", acc);
    return 0;
}
