// vmcheck.c: Lab 5's password check, moved into a toy virtual machine.
// A protector does the same thing at scale: your code becomes bytecode for a VM it invents.
#include <stdio.h>
#include <string.h>

#define KEY 0x37                          /* every bytecode byte is stored XORed with this */

typedef struct { const unsigned char *pc; const char *in; unsigned acc; int eq, done, result; } VM;

static unsigned char fetch(VM *v) { return *v->pc++ ^ KEY; }

static void h_len(VM *v) { v->acc = (unsigned)strlen(v->in); }
static void h_cmp(VM *v) { v->eq = (v->acc == fetch(v)); }
static void h_jne(VM *v) { unsigned char off = fetch(v); if (!v->eq) v->pc += off; }
static void h_ldb(VM *v) { v->acc = (unsigned char)v->in[fetch(v)]; }
static void h_xor(VM *v) { v->acc ^= fetch(v); }
static void h_ret(VM *v) { v->result = fetch(v); v->done = 1; }

static void (*const handlers[])(VM *) = { h_len, h_cmp, h_jne, h_ldb, h_xor, h_ret };

/* 73 bytes, each XOR 0x37 */
static const unsigned char code[] = {
    0x37, 0x36, 0x3F, 0x35, 0x75, 0x34, 0x37, 0x33, 0x6D, 0x36, 0x08, 0x35,
    0x0D, 0x34, 0x36, 0x33, 0x6D, 0x36, 0x1E, 0x35, 0x05, 0x34, 0x35, 0x33,
    0x6D, 0x36, 0x1F, 0x35, 0x1D, 0x34, 0x34, 0x33, 0x6D, 0x36, 0x08, 0x35,
    0x15, 0x34, 0x33, 0x33, 0x6D, 0x36, 0x1B, 0x35, 0x2D, 0x34, 0x32, 0x33,
    0x6D, 0x36, 0x08, 0x35, 0x25, 0x34, 0x31, 0x33, 0x6D, 0x36, 0x1F, 0x35,
    0x3D, 0x34, 0x30, 0x33, 0x6D, 0x36, 0x4C, 0x35, 0x35, 0x32, 0x36, 0x32,
    0x37,
};

static int run(const unsigned char *bytecode, const char *input) {
    VM v = { bytecode, input, 0, 0, 0, 0 };
    while (!v.done)
        handlers[fetch(&v)](&v);          /* fetch, decode, dispatch */
    return v.result;
}

int main(int argc, char **argv) {
    if (argc > 1 && run(code, argv[1])) {
        puts("Correct!");
        return 0;
    }
    puts("Wrong.");
    return 1;
}
