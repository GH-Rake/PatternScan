#include "memhacks.h"

//external patch
void PatchEx(HANDLE hProcess, void* dst, void* src, int size)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, NULL);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}

//internal patch
void Patch(void* dst, void* src, int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void NopEx(HANDLE hProcess, void* dst, unsigned int size)
{
	byte * nopArray = new byte[size];
	memset(nopArray, 0x90, size);

	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, nopArray, size, NULL);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
	delete[] nopArray;
}

void Nop(HANDLE hProcess, void* dst, int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_READWRITE, &oldprotect);
	memset(dst, '0x90', size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}