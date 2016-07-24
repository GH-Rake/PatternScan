#include <Windows.h>
#include "process.h"
#include "patternscan.h"
#include "memhack.h"

//String Conversion Snippet for the sake of knowledge
void StringConversions()
{
	char* c_string = "ac_client.exe";
	wchar_t* wc_converted = TO_WCHAR_T(c_string);
	delete wc_converted;

	wchar_t * wc_string = L"ac_client.exe";
	char * c_converted = TO_CHAR(wc_string);
	delete c_converted;

}

int main()
{
	//String Conversion Snippet
	StringConversions();

	//Get procEntry
	Process ac_clientProc = Process(TEXT("ac_client.exe"));

	//Get handle by OpenProcess
	ac_clientProc.Attach();

	Module ac_clientMod = Module(&ac_clientProc, TEXT("ac_client.exe"));

	if (ac_clientMod.bValid)
	{
		//PatternScan for pattern in ac_client.exe module of ac_client.exe process
		char* healthDecAddress = Pattern::Ex::Mod("\x29\x7b\x00\x8b\xc7", "xx?xx", &ac_clientMod);

		//Scan all modules in process
		healthDecAddress = Pattern::Ex::AllMods("\x29\x7b\x00\x8b\xc7", "xx?xx", &ac_clientProc);

		//Scan module using combo pattern
		healthDecAddress = Pattern::Ex::Mod("29 7b ?? 8b c7", &ac_clientMod);
		
		//Scan all modules using combo pattern
		healthDecAddress = Pattern::Ex::AllMods("29 7b ?? 8b c7", &ac_clientProc);

		//Nop the instructions
		NopEx(healthDecAddress, 5, ac_clientProc.handle);
	}

	return  0;
}