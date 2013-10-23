#include "set_hooks.h"

LPBYTE HookAPI(LPCSTR func_to_hook, LPCSTR module, DWORD trampoline) {
	HMODULE hMod;
	DWORD OldProtect;
	DWORD NewProtect;
	DWORD OriginalFunc;
	DWORD dAddr;
	int i;
	LPBYTE Store;
	LPBYTE p;
	LPBYTE p2;

	hMod = GetModuleHandleA(module);

	OriginalFunc = (DWORD) GetProcAddress(hMod, func_to_hook);

	dAddr = trampoline - OriginalFunc - (5*sizeof(BYTE));
	p = (LPBYTE) OriginalFunc;
	p2 = (LPBYTE) (&dAddr);
	
	NewProtect = PAGE_EXECUTE_READWRITE;
	if(!VirtualProtect((LPVOID) OriginalFunc, 5, NewProtect, &OldProtect)) {
		sprintf(msg, "VirtualProtect error: %d", GetLastError());
		OutputDebugStringA(msg);
		return NULL;
	}

	EnterCriticalSection(&cs);
	Store = (LPBYTE) malloc(5*sizeof(BYTE));
	ZeroMemory(Store, 5*sizeof(BYTE));

	for (i = 0; i < 5*sizeof(BYTE); i += sizeof(BYTE))
		Store[i] = p[i];

	p[0] = (BYTE) 0xE9;

	//sprintf(msg, "HookAPI: Stored 0x%X", Store);
	//OutputDebugStringA(msg);


	for (i = 0; i < 4*sizeof(BYTE); i += sizeof(BYTE)) 
		p[i + 1] = p2[i];

	LeaveCriticalSection(&cs);

	if(!VirtualProtect((LPVOID) OriginalFunc, 5*sizeof(BYTE), OldProtect, &NewProtect)) {
		sprintf(msg, "VirtualProtect2 error: %d", GetLastError());
		OutputDebugStringA(msg);
		return NULL;
	}
	return Store;
}

BOOL UnHookAPI(LPCSTR hooked_func, LPCSTR module, LPBYTE Store) {
	int i;
	DWORD OldProtect;
	DWORD NewProtect;
	DWORD funcToRestore;
	HMODULE hMod;
	LPBYTE p;

	//sprintf(msg, "UnhookAPI: to restore 0x%X", Store);
	OutputDebugStringA(msg);

	hMod = GetModuleHandleA(module);

	funcToRestore = (DWORD) GetProcAddress(hMod, hooked_func);
	
	p = (LPBYTE) funcToRestore;

	NewProtect = PAGE_EXECUTE_READWRITE;
	if(!VirtualProtect((LPVOID) funcToRestore, 5, NewProtect, &OldProtect))
		return FALSE;

	EnterCriticalSection(&cs);

	for (i = 0; i < 5*sizeof(BYTE); i += sizeof(BYTE))
		p[i] = Store[i];

	LeaveCriticalSection(&cs);
	
	free(Store);

	if(!VirtualProtect((LPVOID) funcToRestore, 5, OldProtect, &NewProtect))
		return FALSE;

	return TRUE;
}