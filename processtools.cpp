#include "processtools.h"
#include <stdlib.h>
#include <tchar.h>

//Convert char* to wchar_t*
wchar_t* TO_WCHAR_T(char* string)
{
	unsigned int len = strlen(string) + 1;
	wchar_t* wc_string = new wchar_t[len];
	unsigned int numCharsRead;
	mbstowcs_s(&numCharsRead, wc_string, len, string, _TRUNCATE);
	return wc_string;
}

//Convert wchar_t* to char*
char* TO_CHAR(wchar_t* string)
{
	unsigned int len = wcslen(string) + 1;
	char* c_string = new char[len];
	unsigned int numCharsRead;
	wcstombs_s(&numCharsRead, c_string, len, string, _TRUNCATE);
	return c_string;
}

//Get Process by name
bool Process::Get(TCHAR* exeName, PROCESSENTRY32& procEntry)
{
	procEntry = { 0 };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (hSnapshot)
	{
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnapshot, &procEntry))
		{
			do
			{
				if (!_tcscmp(procEntry.szExeFile, exeName))
				{
					CloseHandle(hSnapshot);
					bValid = true;
					return true;
				}
			} while (Process32Next(hSnapshot, &procEntry));
		}
	}
	CloseHandle(hSnapshot);
	return false;
}

Process::Process(TCHAR* exeName)
{
	this->name = exeName;
	Get(exeName, *this);
}

bool Process::Attach()
{
	handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, th32ProcessID);

	if (handle)
	{
		return true;
	}
	else return false;
}

Module::Module(Process* process, TCHAR* moduleName)
{
	this->name = moduleName;
	this->process = process;
	Get(process, moduleName, *this);
}

bool Module::Get(Process* process, TCHAR* moduleName, MODULEENTRY32 &modEntry)
{
	modEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process->th32ProcessID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		modEntry.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hSnapshot, &modEntry))
		{
			do
			{
				if (!_tcscmp(modEntry.szModule, moduleName))
				{
					CloseHandle(hSnapshot);
					bValid = true;
					return true;
				}
			} while (Module32Next(hSnapshot, &modEntry));
		}
		CloseHandle(hSnapshot);
	}
	bValid = false;
	return false;
}