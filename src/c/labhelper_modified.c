// labhelper_modified.c: labhelper.c plus code someone added. Harmless here: it only logs a line.
#include <windows.h>
#ifndef WHERE
#define WHERE "tools folder"
#endif
__declspec(dllexport) const char *helper_where(void) { return WHERE; }

static void added_on_load(void) {
    char user[64];
    DWORD n = GetEnvironmentVariableA("USERNAME", user, sizeof user);
    OutputDebugStringA(n ? "labhelper: modified build loaded for a named user"
                         : "labhelper: modified build loaded");
}

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) added_on_load();
    return TRUE;
}
