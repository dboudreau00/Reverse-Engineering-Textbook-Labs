// greet_safe.c: the same program, loading its helper the way Microsoft recommends.
#include <stdio.h>
#include <windows.h>

int main(int argc, char **argv) {
    /* From here on, plain LoadLibrary calls search System32 only: no application folder, no PATH. */
    SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_SYSTEM32);
    const char *full = argc > 1 ? argv[1] : "C:\\labtools\\labhelper.dll";   /* a fully qualified path */
    HMODULE h = LoadLibraryExA(full, NULL, 0);
    if (!h) { printf("%s not found (error %lu)\n", full, GetLastError()); return 1; }
    const char *(*where)(void) = (const char *(*)(void))GetProcAddress(h, "helper_where");
    char path[MAX_PATH];
    GetModuleFileNameA(h, path, MAX_PATH);
    printf("loaded %s\nit says: %s\n", path, where ? where() : "(no export)");
    return 0;
}
