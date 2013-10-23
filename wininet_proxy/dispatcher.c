#include "dispatcher.h"

VOID BuildRequestW(LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR *lplpszAcceptTypes, LPCWSTR lpszReferrer, DWORD flags) {
	int i = 0;
	WCHAR tmp[MAX_PATH*sizeof(WCHAR)];	
	ZeroMemory(tmp, MAX_PATH*sizeof(WCHAR));

	if(flags & 0x00800000) //INTERNET_FLAG_SECURE
		wsprintfW(tmp, L"%s https://%s%s HTTP/1.1", lpszVerb, whost, lpszObjectName);
	else
		wsprintfW(tmp, L"%s http://%s%s HTTP/1.1", lpszVerb, whost, lpszObjectName);
	while(lplpszAcceptTypes[i] != NULL) {
		if(!i) {
			wcscat(tmp, L"\r\n\0");
			wcscat(tmp, L"Accept: \0");
		}
		wcscat(tmp, (LPCWSTR) lplpszAcceptTypes[i]);
		wcscat(tmp, L",\0");
		i++;
	}

	if(lpszReferrer) {
		wcscat(tmp, L"\r\nReferrer: \0");
		wcscat(tmp, lpszReferrer);
	}
	if(flags & 0x00400000)  //INTERNET_FLAG_KEEP_CONNECTION
		wcscat(tmp, L"\r\nConnection: Keep-Alive\n\0");

	wrequest = (LPWSTR) malloc(wcslen(tmp)*sizeof(WCHAR));
	wcsncpy(wrequest, tmp, wcslen(tmp));
	return;
}

VOID addVerbW(LPCWSTR verb) {
	wrequest = (LPWSTR) malloc((wcslen(verb) + 1)*sizeof(WCHAR));
	wsprintfW(wrequest, L"%s", verb);
}

VOID addHostW(DWORD flags) {
	WCHAR tmp[MAX_PATH*sizeof(WCHAR)];
	ZeroMemory(tmp, MAX_PATH*sizeof(WCHAR));

	if(flags & 0x00800000) //INTERNET_FLAG_SECURE
		wsprintfW(tmp, L"%s https://%s", wrequest, whost);
	else
		wsprintfW(tmp, L"%s http://%s", wrequest, whost);

	wrequest = (LPWSTR) realloc(wrequest, (wcslen(tmp) + 1)*sizeof(WCHAR));
	wsprintfW(wrequest, L"%s", tmp);
}

VOID addObjectW(LPCWSTR object) {
	LPWSTR tmp;
	tmp = (LPWSTR) malloc(((wcslen(wrequest)*3) + wcslen(object))*sizeof(WCHAR));

	wrequest = (LPWSTR) realloc(wrequest, (wsprintfW(tmp, L"%s%s HTTP/1.1\r\n", wrequest, object) + 1)*sizeof(WCHAR));
	wsprintfW(wrequest, L"%s", tmp);
	free(tmp);
}

VOID addAcceptTypesW(LPCWSTR *types) {
	WCHAR tmp[MAX_PATH*sizeof(WCHAR)];
	int i = 0;
	ZeroMemory(tmp, MAX_PATH*sizeof(WCHAR));

	while(types[i]) {
		if(!i)	
			wsprintfW(tmp, L"Accept: ");
		wcscat(tmp, types[i]);
		wcscat(tmp, L",");
		i++;
	}
	tmp[wcslen(tmp) - 1] = NULL;

	wrequest = (LPWSTR) realloc(wrequest, (wcslen(wrequest) + wcslen(tmp) + 5)*sizeof(WCHAR));
	wcscat(wrequest, tmp);
	wcscat(wrequest, L"\r\n");
}

VOID addReferrerW(LPCWSTR referrer) {
	LPWSTR tmp;
	if(referrer) {
		tmp = (LPWSTR) malloc(((wcslen(wrequest)*2) + wcslen(referrer))*sizeof(WCHAR));
		wsprintfW(tmp, L"%s\r\nReferrer: %s\r\n", wrequest, referrer);

		wrequest = (LPWSTR) realloc(wrequest, (wcslen(tmp) + 1)*sizeof(WCHAR));
		wsprintfW(wrequest, L"%s", tmp);

		free(tmp);
	}
	else
		return;
}

VOID addKeepAliveW(VOID) {
	wrequest = (LPWSTR) realloc(wrequest, (wcslen(wrequest) + (wcslen(L"Connection: Keep-Alive\r\n")) + 1)*sizeof(WCHAR));
	wcscat(wrequest, L"Connection: Keep-Alive\r\n");
}

VOID addHeadersW(LPCWSTR headers) {
	wrequest = (LPWSTR) realloc(wrequest, (wcslen(wrequest) + wcslen(headers) + 5)*sizeof(WCHAR));
	wcscat(wrequest, headers);
	wcscat(wrequest, L"\r\n");
}

VOID addOptionalW(LPVOID optional, DWORD length){
	LPWSTR uni;
	WCHAR tmp[MAX_PATH];
	
	uni = (LPWSTR) malloc(length*sizeof(WCHAR));
	MultiByteToWideChar(CP_OEMCP, 0, (LPCSTR) optional, length, uni, length);
	ZeroMemory(tmp, MAX_PATH);
	

	wsprintfW(tmp, L"Content-Length: %d ---> %d\r\n\r\n", wcslen(uni), length);
	wrequest = (LPWSTR) realloc(wrequest, (wcslen(wrequest) + wcslen(tmp) + wcslen(uni) + 100)*sizeof(WCHAR));
	wcscat(wrequest, tmp);
	wcscat(wrequest, uni);
	wcscat(wrequest, L"\r\n\r\n");
}

VOID addOptionalA(LPVOID optional){
	LPWSTR uni;
	size_t len = strlen((LPCSTR) optional);
	
	uni = (LPWSTR) malloc((len + 1) * sizeof(WCHAR));
	
	MultiByteToWideChar(CP_OEMCP, 0, (LPCSTR) optional, -1, uni, len + 1);

	OutputDebugStringA("+++");
	OutputDebugStringW((LPCWSTR) uni);
	//OutputDebugStringA((LPCSTR) optional);
	OutputDebugStringA("+++");
}