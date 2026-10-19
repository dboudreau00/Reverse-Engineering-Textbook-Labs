// verify.c: asks Windows for the same three addresses by name, to check the hash lookup.
#include <stdio.h>
#include <windows.h>
int main(void) {
    HMODULE k = GetModuleHandleA("kernel32.dll");
    printf("GetProcAddress: GetTickCount=%p GetCurrentProcessId=%p GetSystemTime=%p\n",
           (void *)GetProcAddress(k, "GetTickCount"), (void *)GetProcAddress(k, "GetCurrentProcessId"),
           (void *)GetProcAddress(k, "GetSystemTime"));
    return 0;
}
