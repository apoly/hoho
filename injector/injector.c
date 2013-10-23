#include "injector.h"

BOOL SetPrivilege(HANDLE hToken,LPCSTR lpszPrivilege) {

	TOKEN_PRIVILEGES tp;
	LUID luid;
	if(!LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) {
		printf("LookupPrivilegeValue() failed, error: %u\n", GetLastError());
		return FALSE;
	}
		
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if(!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES) NULL, (PDWORD) NULL)) {
		printf("AdjustTokenPrivileges() failed to adjust the new privilege, error: %u\n", GetLastError());
		return FALSE;
	}
	return TRUE;
}

 
int Inject_DLL(char* dll_to_inject, long pid) {
	HANDLE MyHandle;
	LPVOID MyAlloc;
	int IsWriteOK;
	LPTHREAD_START_ROUTINE addrLoadLibrary;
	DWORD identificateurThread;
	HANDLE ThreadReturn;
	HANDLE hToken;
	long dll_size = strlen(dll_to_inject) + 1;

	printf("-> Getting SE_DEBUG_NAME privilege...\n");

	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		printf("\t [X] OpenProcessToken(): %d\n", GetLastError());
		return 0;
	}
	if(!SetPrivilege(hToken, "SeDebugPrivilege")) {
		printf(" [X] %d\n", GetLastError());
		return 0;
	}

	printf("-> Opening the target process...\n");

	MyHandle = OpenProcess(PROCESS_ALL_ACCESS , FALSE , pid);
	if(MyHandle == NULL) { printf("\t [X] : %d\n", GetLastError()); return 0; }

	printf("-> Memory Allocation...\n");
	MyAlloc = VirtualAllocEx( MyHandle , NULL , dll_size , MEM_COMMIT , PAGE_EXECUTE_READWRITE);
	if(MyAlloc == NULL) { printf("\t [X] : %d\n", GetLastError()); return 0; }

	printf("-> Writing DLL in memory...\n");
	IsWriteOK = WriteProcessMemory(MyHandle, MyAlloc, dll_to_inject, dll_size , 0);
	if(IsWriteOK  == 0) { printf("\t [X] : %d\n", GetLastError()); return 0; }

	printf("-> Creating the Thread...\n");
   
	addrLoadLibrary = (LPTHREAD_START_ROUTINE) GetProcAddress(LoadLibraryA("kernel32"), "LoadLibraryA");
	ThreadReturn = CreateRemoteThread( MyHandle , NULL , 0 , addrLoadLibrary , MyAlloc , 0 , &identificateurThread );
	if(ThreadReturn == NULL) { printf("\t [X] : %d\n", GetLastError()); return 0; }
        
	if ((MyHandle != NULL) && (MyAlloc != NULL) && (IsWriteOK != ERROR_INVALID_HANDLE) && (ThreadReturn != NULL))
		printf("-> DLL injected\n");

     
    return 1;
}

void ProcessToPid(char* process, long pids[]) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32 structprocsnapshot = {0};
	int i = 0;

	structprocsnapshot.dwSize = sizeof(PROCESSENTRY32);

	if(snapshot == INVALID_HANDLE_VALUE)
		return;
	if(Process32First(snapshot,&structprocsnapshot) == FALSE)
		return;

	while(Process32Next(snapshot,&structprocsnapshot)) {
		if(!strcmp(structprocsnapshot.szExeFile,process)) {
			printf("Pid is -> %d\n",structprocsnapshot.th32ProcessID);
			pids[i++] = structprocsnapshot.th32ProcessID;
		}
	}
	CloseHandle(snapshot);
	return;
}

int main(int argc , char* argv[]) {
	long pids[2];
	ProcessToPid("iexplore.exe", pids);
	Inject_DLL("C:\\Users\\apoly\\Documents\\Visual Studio 2012\\Projects\\wininet_proxy\\Debug\\wininet_hooks.dll", pids[0]);
	Inject_DLL("C:\\Users\\apoly\\Documents\\Visual Studio 2012\\Projects\\wininet_proxy\\Debug\\wininet_hooks.dll", pids[1]);
    //Inject_DLL(argv[1], atoi(argv[2]));
}