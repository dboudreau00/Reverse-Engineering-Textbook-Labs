// labhelper.c: a harmless DLL that only reports which copy of it was loaded.
#ifndef WHERE
#define WHERE "tools folder"
#endif
__declspec(dllexport) const char *helper_where(void) { return WHERE; }
