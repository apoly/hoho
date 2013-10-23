#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>

int Inject_DLL(long pidProcAInjecter , char* dll_to_inject);
void ProcessToPid(char* process, long pids[]);