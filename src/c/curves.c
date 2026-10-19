// curves.c: stores two curves' constants the way crypto libraries do. It computes nothing.
#include <stdint.h>
#include <stdio.h>

/* NIST P-256 coefficient b: 32 big-endian bytes, the order used in specifications */
static const uint8_t P256_B[32] = {
    0x5a, 0xc6, 0x35, 0xd8, 0xaa, 0x3a, 0x93, 0xe7, 0xb3, 0xeb, 0xbd, 0x55, 0x76, 0x98, 0x86, 0xbc,
    0x65, 0x1d, 0x06, 0xb0, 0xcc, 0x53, 0xb0, 0xf6, 0x3b, 0xce, 0x3c, 0x3e, 0x27, 0xd2, 0x60, 0x4b,
};

/* secp256k1 generator x: four 64-bit limbs, least significant first, as many bignum libraries store it */
static const uint64_t K1_GX[4] = {
    0x59F2815B16F81798ULL, 0x029BFCDB2DCE28D9ULL, 0x55A06295CE870B07ULL, 0x79BE667EF9DCBBACULL,
};

int main(void) {
    /* write the raw constants so the compiler must keep both arrays in the binary */
    fwrite(P256_B, 1, sizeof P256_B, stdout);
    fwrite(K1_GX, 1, sizeof K1_GX, stdout);
    return 0;
}
