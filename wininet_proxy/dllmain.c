#include "set_hooks.h"

BOOL APIENTRY DllMain (HINSTANCE hInst,DWORD reason, LPVOID reserved) {

	   
	switch (reason) {
		case DLL_PROCESS_ATTACH:

			InitializeCriticalSection(&cs);

			/*OutputDebugStringA("hook dll loaded!");
			if((_SendReqA = HookAPI("HttpSendRequestA", "wininet.dll", (DWORD) _HttpSendRequestA)) != NULL) 
				OutputDebugStringA("HttpSendRequestA hooked");
			if((_SendReqW = HookAPI("HttpSendRequestW", "wininet.dll", (DWORD) _HttpSendRequestW)) != NULL) 
				OutputDebugStringA("HttpSendRequestW hooked");
			
			if((_InternetConA = HookAPI("InternetConnectA", "wininet.dll", (DWORD) _InternetConnectA)) != NULL)
				OutputDebugStringA("InternetConnectA hooked");
			if((_InternetConW = HookAPI("InternetConnectW", "wininet.dll", (DWORD) _InternetConnectW)) != NULL)
				OutputDebugStringA("InternetConnectW hooked");

			if((_OpenReqA = HookAPI("HttpOpenRequestA", "wininet.dll", (DWORD) _HttpOpenRequestA)) != NULL)
				OutputDebugStringA("HttpOpenRequestA hooked");
			if((_OpenReqW = HookAPI("HttpOpenRequestW", "wininet.dll", (DWORD) _HttpOpenRequestW)) != NULL)
				OutputDebugStringA("HttpOpenRequestW hooked");

			if((_EncryptMsg = HookAPI("EncryptMessage", "secur32.dll", (DWORD) _EncryptMessage)) != NULL)
				OutputDebugStringA("encryptmessage hooked");
			if((_DecryptMsg = HookAPI("DecryptMessage", "secur32.dll", (DWORD) _DecryptMessage)) != NULL)
				OutputDebugStringA("decryptmessage hooked");*/

			if((_Close = HookAPI("InternetCloseHandle", "wininet.dll", (DWORD) _InternetCloseHandle)) != NULL) 
				OutputDebugStringA("InternetCloseHandle hooked");

		break;
	}
	return TRUE;
}