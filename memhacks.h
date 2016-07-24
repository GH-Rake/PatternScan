#include <windows.h>

//patch internal
void Patch(void* dst, void* src, int size);

//external
void PatchEx(HANDLE hProcess, void* dst, void* src, int size);

//Nop Internal
void Nop(HANDLE hProcess, void* dst, int size);

//External
void NopEx(HANDLE hProcess, void* dst, unsigned int size);