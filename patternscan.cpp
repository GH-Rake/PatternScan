#include "patternscan.h"

//health sig = 29 7b 04 8b c7
void ParsePattern(char* combo, char * pattern, char * mask)
{
	unsigned int patternLen = (strlen(combo) + 1) / 3;

	for (unsigned int i = 0; i < strlen(combo); i++)
	{
		if (combo[i] == ' ')
		{
			continue;
		}

		else if (combo[i] == '?')
		{
			mask[(i + 1) / 3] = '?';
			i += 2;
		}

		else
		{
			char byte = (char)strtol(&combo[i], 0, 16);
			pattern[(i + 1) / 3] = byte;
			mask[(i + 1) / 3] = 'x';
			i += 2;
		}
	}
	pattern[patternLen] = '\0';
	mask[patternLen] = '\0';
}

//Internal Pattern scan, external pattern scan is just a wrapper around this
void* PatternScan(char* base, size_t size, char* pattern, char *mask)
{
	size_t patternLen = strlen(pattern);

	for (unsigned int i = 0; i < size - patternLen; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLen; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(base + i + j))
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return (void*)(base + i);
		}
	}
	return nullptr;
}

//basic function
void* PatternScanEx(HANDLE hProcess, uintptr_t begin, uintptr_t end, char *pattern, char *mask)
{
	uintptr_t currentChunk = begin;
	//uintptr_t end = currentChunk + size;
	SIZE_T bytesRead;

	while (currentChunk < end)
	{
		//local storage of external memory
		byte buffer[4096];

		//make data accessible to ReadProcessMemory
		DWORD oldprotect;
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), PROCESS_ALL_ACCESS, &oldprotect);

		//Copy chunk of external memory into local storage

		ReadProcessMemory(hProcess, (void*)currentChunk, &buffer, sizeof(buffer), &bytesRead);

		//if readprocessmemory failed, return
		if (bytesRead == 0)
		{
			return nullptr;
		}

		//Find pattern in local buffer, if pattern is found return address of matching data
		void* InternalAddress = PatternScan((char*)&buffer, bytesRead, pattern, mask);

		//if Find Pattern returned an address
		if (InternalAddress != 0)
		{
			//convert internal offset to external address and return
			uintptr_t offsetFromBuffer = (uintptr_t)InternalAddress - (uintptr_t)&buffer;
			return (void*)(currentChunk + offsetFromBuffer);
		}

		//pattern not found in this chunk
		else
		{
			//advance to next chunk
			currentChunk = currentChunk + bytesRead;
		}
	}

	return 0;
}

//module scan wrapper
void* PatternScanExModule(HANDLE hProcess, wchar_t *module, char *pattern, char *mask)
{
	//Grab module information from External Process

	DWORD processID = GetProcessId(module);
	MODULEENTRY32 modEntry = GetModule(processID, module);
	uintptr_t begin = (uintptr_t)modEntry.modBaseAddr;
	uintptr_t end = begin + modEntry.modBaseSize;

	return PatternScanEx(hProcess, begin, end, pattern, mask);
}

void* PatternScanExProcess(wchar_t* processName, char *pattern, char *mask)
{
	PROCESSENTRY32 procEntry = GetProcess(processName);
	HANDLE hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, procEntry.th32ProcessID);

	MODULEENTRY32 modEntry = { 0 };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procEntry.th32ProcessID);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		modEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &modEntry))
		{
			do
			{
				void* patternAddress = PatternScan((char*)modEntry.modBaseAddr, modEntry.dwSize, pattern, mask);
				if (patternAddress != nullptr)
				{
					CloseHandle(hSnapshot);
					return (void*)patternAddress;
				}
			} while (Module32Next(hSnapshot, &modEntry));
		}
		CloseHandle(hSnapshot);
	}
	return nullptr;
}

//Combo Module Wrapper
//health sig = 29 7b 04 8b c7
void* PatternScanExCombo(HANDLE hProcess, wchar_t *module, char *combopattern)
{
	unsigned int patternLen = ((strlen(combopattern) + 1) / 3) + 1;
	char* pattern = new char[patternLen];
	char* mask = new char[patternLen];

	ParsePattern(combopattern, pattern, mask);

	void* match = PatternScanExModule(hProcess, module, pattern, mask);

	delete[] pattern;
	delete[] mask;
	return match;
}