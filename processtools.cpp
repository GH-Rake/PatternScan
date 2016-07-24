#include "processtools.h"

DWORD GetProcID(wchar_t* exeName)
{
	PROCESSENTRY32 procEntry = { 0 };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!hSnapshot)
		return NULL;

	procEntry.dwSize = sizeof(procEntry);

	if (!Process32First(hSnapshot, &procEntry))
		return NULL;

	do
	{
		if (!wcscmp(procEntry.szExeFile, exeName))
		{
			CloseHandle(hSnapshot);
			return procEntry.th32ProcessID;
		}
	} while (Process32Next(hSnapshot, &procEntry));

	CloseHandle(hSnapshot);
	return NULL;
}

MODULEENTRY32 GetModule(DWORD dwProcID, wchar_t* moduleName)
{
	MODULEENTRY32 modEntry = { 0 };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcID);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		modEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &modEntry))
		{
			do
			{
				if (wcscmp(modEntry.szModule, moduleName) == 0)
				{
					break;
				}
			} while (Module32Next(hSnapshot, &modEntry));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}

PROCESSENTRY32 GetProcess(wchar_t* exeName)
{
	PROCESSENTRY32 procEntry = { 0 };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!hSnapshot)
		return procEntry;

	procEntry.dwSize = sizeof(procEntry);

	if (!Process32First(hSnapshot, &procEntry))
		return procEntry;

	do
	{
		if (!wcscmp(procEntry.szExeFile, exeName))
		{
			CloseHandle(hSnapshot);
			return procEntry;
		}
	} while (Process32Next(hSnapshot, &procEntry));

	CloseHandle(hSnapshot);
	return procEntry;
}