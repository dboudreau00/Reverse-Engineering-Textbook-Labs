// apihash.c: finds Windows API functions by a hash of their name instead of by name.
#include <stdint.h>
#include <stdio.h>
#include <windows.h>

static uint32_t fnv1a(const char *s) {
    uint32_t h = 2166136261u;                       /* FNV offset basis 0x811C9DC5 */
    while (*s) { h ^= (uint8_t)*s++; h *= 16777619u; }  /* FNV prime 0x01000193 */
    return h;
}

static void *resolve(HMODULE mod, uint32_t want) {  /* walk the export table, compare hashes */
    BYTE *base = (BYTE *)mod;
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)(base + ((IMAGE_DOS_HEADER *)base)->e_lfanew);
    IMAGE_DATA_DIRECTORY dir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    IMAGE_EXPORT_DIRECTORY *ex = (IMAGE_EXPORT_DIRECTORY *)(base + dir.VirtualAddress);
    DWORD *names = (DWORD *)(base + ex->AddressOfNames);
    WORD *ords = (WORD *)(base + ex->AddressOfNameOrdinals);
    DWORD *funcs = (DWORD *)(base + ex->AddressOfFunctions);
    for (DWORD i = 0; i < ex->NumberOfNames; i++)
        if (fnv1a((const char *)(base + names[i])) == want)
            return base + funcs[ords[i]];
    return NULL;
}

int main(void) {
    HMODULE k32 = GetModuleHandleA("kernel32.dll");
    static const struct { uint32_t hash; const char *label; } wanted[] = {
        { 0x0F4D8B55u, "function A" },
        { 0x020A0780u, "function B" },
        { 0x03595AAFu, "function C" },
    };
    for (int i = 0; i < 3; i++) {
        void *p = resolve(k32, wanted[i].hash);
        printf("%s: hash 0x%08X -> %p\n", wanted[i].label, (unsigned)wanted[i].hash, p);
    }
    return 0;
}
