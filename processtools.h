#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

class Module;

class Process : public PROCESSENTRY32
{
public:
	TCHAR* name;
	HANDLE handle = 0;
	bool bValid = false;

	//std::vector<Module>* modules;

	Process(TCHAR* exeName);
	bool Get(TCHAR* exeName, PROCESSENTRY32 &procEntry);
	bool Attach();
};

class Module : public MODULEENTRY32
{
public:
	TCHAR* name;
	bool bValid = false;
	Process * process;

	Module(Process* process, TCHAR* moduleName);
	bool Get(Process* process, TCHAR* moduleName, MODULEENTRY32 &modEntry);
};


//String conversion snippets

//Convert char* to wchar_t*
wchar_t* TO_WCHAR_T(char* string);

//Convert wchar_t* to char*
char* TO_CHAR(wchar_t* string);