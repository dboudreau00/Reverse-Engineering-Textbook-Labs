// greet.c: loads labhelper.dll by bare name, the risky way.
#include <stdio.h>
#include <windows.h>

int main(void) {
    HMODULE h = LoadLibraryA("labhelper.dll");                 /* Windows searches for it */
    if (!h) { printf("labhelper.dll not found (error %lu)\n", GetLastError()); return 1; }
    const char *(*where)(void) = (const char *(*)(void))GetProcAddress(h, "helper_where");
    char path[MAX_PATH];
    GetModuleFileNameA(h, path, MAX_PATH);
    printf("loaded %s\nit says: %s\n", path, where ? where() : "(no export)");
    return 0;
}
