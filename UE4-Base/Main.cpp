#include "Includes.h"

HMODULE mod;
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lReserved)
{
	mod = hModule;
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		Hooks::StartUp();
	}

	return TRUE;
}


