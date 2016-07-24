#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include "processtools.h"

namespace Pattern
{
	//Split combo pattern into mask/pattern
	void Parse(char* combo, char* pattern, char* mask);

	namespace In
	{
		//Internal Pattern Scan
		char* Scan(char* pattern, char* mask, char* begin, unsigned int size);

		char* Mod(char *combopattern, Module*  module);

		char* AllMods(char* combopattern);

		char* Proc(char* combopattern);
	}

	namespace Ex
	{
		//External Wrapper
		char* Scan(char* pattern, char* mask, char* begin, char* end, Process* process);

		//Scan just a module
		char* Mod(char* pattern, char* mask, Module* module);
		//Overloaded Function for combopattern
		char* Mod(char* combopattern, Module* module);

		//Scan all modules from process
		char* AllMods(char* pattern, char* mask, Process* process);
		//Overloaded Function for combopattern
		char* AllMods(char* combopattern, Process* process);

		//Scan entire process
		char* Proc(char* combopattern, Process* process);

	}
}