"""Builds 80 small Windows programs (40 packed with UPX) and writes dataset.csv."""
import csv
import os
import random
import subprocess

from pefeatures import NAMES, features

random.seed(12)
FUNCS = ["xor_decode", "rc4_ksa", "rc4_prga", "fnv1a32", "crc32_bitwise", "base64_encode",
         "bubble_sort", "str_len", "reverse_string", "popcount32", "gcd", "is_prime"]
CALLS = {
    "xor_decode": "xor_decode(buf, sizeof buf, 0x5A);",
    "rc4_ksa": "rc4_ksa(S, buf, 8);",
    "rc4_prga": "rc4_prga(S, buf, sizeof buf);",
    "fnv1a32": "acc ^= fnv1a32(buf, sizeof buf);",
    "crc32_bitwise": "acc ^= crc32_bitwise(buf, sizeof buf);",
    "base64_encode": "acc ^= (uint32_t)base64_encode(buf, 48, out);",
    "bubble_sort": "bubble_sort(nums, 16);",
    "str_len": "acc ^= (uint32_t)str_len(out);",
    "reverse_string": "reverse_string(out, 16);",
    "popcount32": "acc ^= (uint32_t)popcount32(acc);",
    "gcd": "acc ^= gcd(acc | 1, 360);",
    "is_prime": "acc ^= (uint32_t)is_prime(acc);",
}
os.makedirs("samples", exist_ok=True)
rows = []
for n in range(40):
    chosen = random.sample(FUNCS, random.randint(3, 8))
    blob = n % 2 == 1                       # half carry 32 KB of random bytes: high entropy, NOT packed
    src = ['#include "corpus.c"', "#include <stdio.h>"]
    if blob:
        data = ", ".join(str(random.randrange(256)) for _ in range(32768))
        src.append(f"static const uint8_t BLOB[32768] = {{{data}}};")
    src += ["int main(int argc, char **argv) {",
            "    uint8_t buf[64] = {0}, S[256]; char out[128]; int nums[16]; uint32_t acc = (uint32_t)argc;",
            "    for (int i = 0; i < 16; i++) nums[i] = (int)(acc * 2654435761u >> (i % 24));",
            "    for (int i = 0; i < 64; i++) buf[i] = (uint8_t)(argv[0][i % 8] + i);"]
    if blob:
        src.append("    for (int i = 0; i < 32768; i += 97) acc += BLOB[i];")
    src += ["    " + CALLS[f] for f in chosen]
    src += ['    printf("%u\\n", acc);', "    return 0;", "}"]
    c = f"samples/prog{n:02}.c"
    open(c, "w").write("\n".join(src) + "\n")
    exe = f"samples/prog{n:02}.exe"
    subprocess.run(["x86_64-w64-mingw32-gcc", "-O2", "-s", "-I.", "-o", exe, c], check=True)
    packed = f"samples/prog{n:02}_upx.exe"
    subprocess.run(["upx", "-q", "--best", "-o", packed, exe], check=True, capture_output=True)
    for path, label in ((exe, 0), (packed, 1)):
        rows.append([path, int(blob), label] + [round(v, 4) for v in features(path)])

with open("dataset.csv", "w", newline="") as f:
    w = csv.writer(f)
    w.writerow(["path", "has_blob", "packed"] + NAMES)
    w.writerows(rows)
print(len(rows), "rows written")
