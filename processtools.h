#pragma once
#include <Windows.h>
#include <TlHelp32.h>

PROCESSENTRY32 GetProcess(wchar_t* exeName);

MODULEENTRY32 GetModule(DWORD dwProcID, wchar_t* moduleName);

DWORD GetProcID(wchar_t* exeName);