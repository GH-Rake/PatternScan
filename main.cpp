//http://guidedhacking.com
#include <Windows.h>
#include <TlHelp32.h>
#include "process.h"
#include "patternscan.h"
#include "memhacks.h"

int main()
{
	DWORD processID = GetProcID(L"ac_client.exe");

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);

	void* healthDecAddress1 = PatternScanExModule(hProcess, L"ac_client.exe", "\x29\x7b\x00\x8b\xc7", "xx?xx");

	void* healthDecAddress = PatternScanExCombo(hProcess, L"ac_client.exe", "29 7b ?? 8b c7");

	NopEx(hProcess, healthDecAddress, 5);

	CloseHandle(hProcess);

	return 0;
}