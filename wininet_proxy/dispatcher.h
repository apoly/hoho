#ifndef __DISPATCHER__
#define __DISPATCHER__

#include <Windows.h>
#include <WinSock.h>
#include <wchar.h>

#pragma comment(lib, "ws2_32")

LPWSTR wrequest;
WCHAR whost[MAX_PATH];


VOID addVerbW(LPCWSTR verb);
VOID addHostW(DWORD flags);
VOID addObjectW(LPCWSTR object);
VOID addAcceptTypesW(LPCWSTR *types);
VOID addReferrerW(LPCWSTR referrer);
VOID addKeepAliveW(VOID);
VOID addHeadersW(LPCWSTR headers);
VOID addOptionalW(LPVOID optional, DWORD length);
VOID addOptionalA(LPVOID optional, DWORD length);


VOID BuildRequestW(LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR *lplpszAcceptTypes, LPCWSTR lpszReferrer, DWORD flags);

#endif

