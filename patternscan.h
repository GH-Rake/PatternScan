#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include "processtools.h"

//Combined pattern & mask
void ParsePattern(char* combo, char * pattern, char * mask);

//Internal Pattern scan, external pattern scan is just a wrapper around this
void* PatternScan(char* base, size_t size, char* pattern, char *mask);

//External Wrapper for internal pattern scan
void* PatternScanEx(HANDLE hProcess, uintptr_t begin, uintptr_t end, char *pattern, char *mask);

//External version
void* PatternScanExModule(HANDLE hProcess, wchar_t *module, char *pattern, char *mask);

//External all modules
void* PatternScanExProcess(wchar_t* processName, char *pattern, char *mask);

//Combined pattern * mask scan
void* PatternScanExCombo(HANDLE hProcess, wchar_t *module, char *combopattern);