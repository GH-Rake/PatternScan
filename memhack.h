#pragma once
#include <Windows.h>

//internal patch
void Patch(char* dst, char* src, unsigned int size);

//external
void PatchEx(char* dst, char* src, unsigned int size, HANDLE hProcess);

//Internal Nop
void Nop(char* dst, unsigned int size);

//External
void NopEx(char* dst, unsigned int size, HANDLE hProcess);