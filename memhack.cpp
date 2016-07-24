#include "memhack.h"

//internal patch
void Patch(char* dst, char* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

//external
void PatchEx(char* dst, char* src, unsigned int size, HANDLE hProcess)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, NULL);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}

//Internal Nop
void Nop(char* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}


//External
void NopEx(char* dst, unsigned int size, HANDLE hProcess)
{
	byte* nopArray = new byte[size];
	memset(nopArray, 0x90, size);

	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, nopArray, size, NULL);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
	delete[] nopArray;
}