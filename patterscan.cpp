#include "patternscan.h"

//Split combo pattern into mask/pattern
void Pattern::Parse(char* combo, char* pattern, char* mask)
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

//Internal Pattern Scan
char* Pattern::In::Scan(char* pattern, char* mask, char* begin, unsigned int size)
{
	unsigned int patternLength = strlen(pattern);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] !=*(begin + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return (begin + i);
		}
	}
	return nullptr;
}

char* Pattern::In::Mod(char *combopattern, Module* module)
{


}

char* Pattern::In::AllMods(char* combopattern)
{

}

char* Pattern::In::Proc(char* combopattern)
{

}

//External Wrapper
char* Pattern::Ex::Scan(char* pattern, char* mask, char* begin, char* end, Process* process)
{
	char* currentChunk = begin;
	SIZE_T bytesRead;

	while (currentChunk < end)
	{
		char buffer[4096]; //char * buffer[4096];?

		DWORD oldprotect;
		VirtualProtectEx(process->handle, currentChunk, sizeof(buffer), PROCESS_VM_READ, &oldprotect);
		ReadProcessMemory(process->handle, currentChunk, &buffer, sizeof(buffer), &bytesRead);
		VirtualProtectEx(process->handle, currentChunk, sizeof(buffer), oldprotect, NULL);

		if (bytesRead == 0)
		{
			return nullptr;
		}

		char* internalAddress = In::Scan(pattern, mask, (char*)&buffer, bytesRead);

		if (internalAddress != nullptr)
		{
			//calculate from internal to external
			uintptr_t offsetFromBuffer = internalAddress - (char*)&buffer;
			return (currentChunk + offsetFromBuffer);
		}
		else
		{
			//advance to next chunk
			currentChunk = currentChunk + bytesRead;
		}
	}
	return nullptr;
}

//Module wrapper for external pattern scan
char* Pattern::Ex::Mod(char* pattern, char* mask, Module* module)
{
	return Scan(pattern, mask, (char*)module->modBaseAddr, (char*)module->modBaseAddr + module->modBaseSize, module->process);
}

//loops through all modules and scans them
char* Pattern::Ex::AllMods(char* pattern, char* mask, Process* process)
{
	MODULEENTRY32 modEntry = { 0 };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process->th32ProcessID);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		modEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &modEntry))
		{
			do
			{
				char* patternAddress = In::Scan(pattern, mask, (char*)modEntry.modBaseAddr, modEntry.dwSize);
				if (patternAddress != nullptr)
				{
					CloseHandle(hSnapshot);
					return patternAddress;
				}
			} while (Module32Next(hSnapshot, &modEntry));
		}
		CloseHandle(hSnapshot);
	}
	return nullptr;
}

//Combo External Module
char* Pattern::Ex::Mod(char* combopattern, Module* module)
{
	unsigned int patternLen = ((strlen(combopattern) + 1) / 3) + 1;
	char* pattern = new char[patternLen];
	char* mask = new char[patternLen];

	Parse(combopattern, pattern, mask);

	char* match = Pattern::Ex::Mod(pattern, mask, module);
	
	delete[] pattern;
	delete[] mask;
	return match;
}

//Combo External Process
char* Pattern::Ex::AllMods(char* combopattern, Process* process)
{
	unsigned int patternLen = ((strlen(combopattern) + 1) / 3) + 1;
	char* pattern = new char[patternLen];
	char* mask = new char[patternLen];

	Parse(combopattern, pattern, mask);

	char* match = AllMods(pattern, mask, process);

	delete[] pattern;
	delete[] mask;
	return match;
}