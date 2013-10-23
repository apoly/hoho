#ifndef __SET_HOOKS__
#define __SET_HOOKS__

#include <Windows.h>
#include <stdio.h>
#include "hook_funcs.h"

CRITICAL_SECTION cs;

LPBYTE HookAPI(LPCSTR func_to_hook, LPCSTR module, DWORD trampoline);
BOOL UnHookAPI(LPCSTR hooked_func, LPCSTR module, LPBYTE Store);



#endif