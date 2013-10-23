#include "hook_funcs.h"


BOOL WINAPI _HttpSendRequestA(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength) {
	BOOL ret;
	sprintf(msg, "_HttpSendRequestA - %s", lpszHeaders);
	OutputDebugStringA(msg);
	if(lpOptional) {
		sprintf(msg, "POST: %s", (LPSTR) lpOptional);
		OutputDebugStringA(msg);
	}
	UnHookAPI("HttpSendRequestA", "wininet.dll", _SendReqA);
	ret = HttpSendRequestA(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
	_SendReqA = HookAPI("HttpSendRequestA", "wininet.dll", (DWORD) _HttpSendRequestA);
	return ret;
}

BOOL WINAPI _HttpSendRequestW(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength) {
	BOOL ret;
	WCHAR tmp[MAX_PATH*sizeof(WCHAR)];
	/*wsprintfW((LPWSTR) msg, L"_HttpSendRequestW - %s", lpszHeaders);
	OutputDebugStringW((LPWSTR) msg);

	ZeroMemory(tmp, MAX_PATH*sizeof(WCHAR));

	if(lpszHeaders) {
		wcscpy(tmp, L"\r\n\0");
		wcscat(tmp, lpszHeaders);
	}

	if(lpOptional) {
		//sprintf(msg, "POST: %s", (LPSTR) lpOptional);
		//OutputDebugStringA(msg);
		wsprintfW((LPWSTR) msg, L"\r\nContent-Length: %d", dwOptionalLength);
		wcscat(tmp, (LPCWSTR) msg);
		
		wcscat(tmp, L"\r\n\0");
		wcsncat(tmp, (LPCWSTR) lpOptional, dwOptionalLength);

		wcscat(tmp, L"\r\n\r\n\0");
	}

	if(wcslen(tmp)) {
		wrequest = (LPWSTR) realloc(wrequest, wcslen(wrequest) + wcslen(tmp));
		wcsncat(wrequest, tmp, wcslen(tmp));
	}

	OutputDebugStringW(L"\n");
	OutputDebugStringW(wrequest);
	free(wrequest);
	*/ 

	if(dwHeadersLength)
		addHeadersW(lpszHeaders);
	
	if(dwOptionalLength)
		addOptionalW(lpOptional, dwOptionalLength);

	OutputDebugStringW(L"\n");
	OutputDebugStringW(wrequest);
	OutputDebugStringW(L"\n");
	free(wrequest);
	
	UnHookAPI("HttpSendRequestW", "wininet.dll", _SendReqW);
	ret = HttpSendRequestW(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
	_SendReqW = HookAPI("HttpSendRequestW", "wininet.dll", (DWORD) _HttpSendRequestW);
	return ret;
}

HINTERNET WINAPI _InternetConnectA(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUsername, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext) {
	HINTERNET ret;
	/*__asm {
		int 3;
	}*/

	sprintf(msg, "_InternetConnectA - %s", lpszServerName);
	OutputDebugStringA(msg);
	
	UnHookAPI("InternetConnectA", "wininet.dll", _InternetConA);
	ret = InternetConnectA(hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
	_InternetConA = HookAPI("InternetConnectA", "wininet.dll", (DWORD) _InternetConnectA);
	return ret;
}

HINTERNET WINAPI _InternetConnectW(HINTERNET hInternet, LPCWSTR lpszServerName, INTERNET_PORT nServerPort, LPCWSTR lpszUsername, LPCWSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext) {
	HINTERNET ret;
	/*__asm {
		int 3;
	}
	wsprintfW((LPWSTR) msg, L"_InternetConnectW - %s", lpszServerName);
	OutputDebugStringW((LPWSTR) msg);*/

	ZeroMemory(whost, MAX_PATH);
	wcscpy(whost, lpszServerName);

	UnHookAPI("InternetConnectW", "wininet.dll", _InternetConW);
	ret = InternetConnectW(hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
	_InternetConW = HookAPI("InternetConnectW", "wininet.dll", (DWORD) _InternetConnectW);
	return ret;
}

HINTERNET WINAPI _HttpOpenRequestA(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferrer, LPCSTR *lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext) {
	HINTERNET ret;
	/*__asm {
		int 3;
	}
	sprintf(msg, "_HttpOpenRequestA: %s | %s | %s | %s | %s", lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes[0]);
	OutputDebugStringA(msg);*/

	UnHookAPI("HttpOpenRequestA", "wininet.dll", _OpenReqA);
	ret = HttpOpenRequestA(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext);
	_OpenReqA = HookAPI("HttpOpenRequestA", "wininet.dll", (DWORD) _HttpOpenRequestA);
	return ret;
}

HINTERNET WINAPI _HttpOpenRequestW(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR lpszVersion, LPCWSTR lpszReferrer, LPCWSTR *lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext) {
	HINTERNET ret;
	/*__asm {
		int 3;
	}
	wsprintfW((LPWSTR) msg, L"_HttpOpenRequestW: %s | %s | %s | %s | %s", lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes[0]);
	OutputDebugStringW((LPWSTR) msg);

	BuildRequestW(lpszVerb, lpszObjectName, lplpszAcceptTypes, lpszReferrer, dwFlags);*/

	
	addVerbW(lpszVerb);
	addHostW(dwFlags);
	addObjectW(lpszObjectName);
	addAcceptTypesW(lplpszAcceptTypes);
	addReferrerW(lpszReferrer);
	if(dwFlags & INTERNET_FLAG_KEEP_CONNECTION)
		addKeepAliveW();
	

	UnHookAPI("HttpOpenRequestW", "wininet.dll", _OpenReqW);
	ret = HttpOpenRequestW(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext);
	_OpenReqW = HookAPI("HttpOpenRequestW", "wininet.dll", (DWORD) _HttpOpenRequestW);
	return ret;
}

BOOL _InternetCloseHandle(HINTERNET hInternet) {
	BOOL ret;
	LPVOID data;
	DWORD size;

	data = malloc(10240*sizeof(BYTE));
	ZeroMemory(data, 10240*sizeof(BYTE));
	size = 0;
	HttpQueryInfoA(hInternet, HTTP_QUERY_RAW_HEADERS_CRLF, data, &size, 0);
	if(size)
		OutputDebugStringA((LPCSTR) data);
	free(data);	

	UnHookAPI("InternetCloseHandle", "wininet.dll", _Close);
	ret = InternetCloseHandle(hInternet);
	_Close = HookAPI("InternetCloseHandle", "wininet.dll", (DWORD) _InternetCloseHandle);
	return ret;

}

SECURITY_STATUS SEC_ENTRY _EncryptMessage(PCtxtHandle phContext, ULONG fQOP, PSecBufferDesc pMessage, ULONG MessageSeqNo) {
	SECURITY_STATUS ret;
	LPBYTE data;
	int i;
	
	if(pMessage) {
		i = pMessage->cBuffers;

		while(i--)
			if(pMessage->pBuffers[i].BufferType == SECBUFFER_DATA) {
				data = (LPBYTE) malloc(pMessage->pBuffers[i].cbBuffer*sizeof(BYTE) + 1);
				strncpy((LPSTR) data, (LPCSTR) pMessage->pBuffers[i].pvBuffer, pMessage->pBuffers[i].cbBuffer);
				data[pMessage->pBuffers[i].cbBuffer] = NULL;
				OutputDebugStringA((LPCSTR) data);
				free(data);
			}
	}

	UnHookAPI("EncryptMessage", "secur32.dll", _EncryptMsg);
	ret = EncryptMessage(phContext, fQOP, pMessage, MessageSeqNo);
	_EncryptMsg = HookAPI("EncryptMessage", "secur32.dll", (DWORD) _EncryptMessage);
	return ret;
}

SECURITY_STATUS SEC_ENTRY _DecryptMessage(PCtxtHandle phContext, PSecBufferDesc pMessage, ULONG MessageSeqNo, PULONG pfQOP) {
	SECURITY_STATUS ret;
	LPBYTE data;
	int i;
	
	if(pMessage)
	 i = pMessage->cBuffers;


	UnHookAPI("DecryptMessage", "secur32.dll", _DecryptMsg);
	ret = DecryptMessage(phContext, pMessage, MessageSeqNo, pfQOP);
	_DecryptMsg = HookAPI("DecryptMessage", "secur32.dll", (DWORD) _DecryptMessage);
	
	if(pMessage)
		while(i--)
			if(pMessage->pBuffers[i].BufferType == SECBUFFER_DATA || pMessage->pBuffers[i].BufferType == SECBUFFER_EMPTY) {
				data = (LPBYTE) malloc(pMessage->pBuffers[i].cbBuffer*sizeof(BYTE) + 1);
				strncpy((LPSTR) data, (LPCSTR) pMessage->pBuffers[i].pvBuffer, pMessage->pBuffers[i].cbBuffer);
				data[pMessage->pBuffers[i].cbBuffer] = NULL;
				OutputDebugStringA((LPCSTR) data);
				free(data);
			}

	return ret;
}
