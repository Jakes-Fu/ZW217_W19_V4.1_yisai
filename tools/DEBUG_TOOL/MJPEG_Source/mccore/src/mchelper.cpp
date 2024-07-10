#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "mchelper.h"

void CArgs::Set(char* string)
{
	int i;
	char *p;
	argv = 0;
	data = strdup(string);
	p = data;
	for (i = 0; ; i++) {
		char *s;
		argv = (char**)realloc(argv, (i + 1) * sizeof(char*));
		if (*p == '\"') {
			argv[i] = ++p;
			s = strchr(p, '\"');
			if (!s) break;
			*s = 0;
		} else {
			argv[i] = p;
			s = strchr(p, ' ');
			if (!s) break;
			*s = 0;
		}
		while (*(++s) == ' ');
		p = s;
		if (!p) break;
	}
	argc = i + 1;
}

HMODULE CDllHelper::LoadDLL(TCHAR* path, TCHAR* file)
{
	TCHAR curdir[MAX_PATH];
	if (!path) return 0;
	GetCurrentDirectory(sizeof(curdir), curdir);
	if (!SetCurrentDirectory(path)) return 0;
	hDll = LoadLibrary(file);
	SetCurrentDirectory(curdir);
	if (!hDll) return 0;
	pfnmain = (PFNMAIN)GetProcAddress(hDll, "main");
	return hDll;
}

HMODULE CDllHelper::LoadDLL(TCHAR* file)
{
	if (hDll) return hDll;
	hDll = LoadLibrary(file);
	if (!hDll) return 0;
	pfnmain = (PFNMAIN)GetProcAddress(hDll, "main");
	return hDll;
}

void CDllHelper::Unload()
{	
	if (hDll) FreeLibrary(hDll);
}

DWORD WINAPI CDllHelper::MainThread(void* ptrthis)
{
	CDllHelper* dll = (CDllHelper*)ptrthis;
	return (*dll->pfnmain)(dll->args->argc, dll->args->argv);
}

int CDllHelper::StartMain(char* cmdline)
{
	DWORD dwid;
	if (!pfnmain) return -1;
	if (!args) args = new CArgs;
	args->Set(cmdline);
	hThread = CreateThread(0,0,(LPTHREAD_START_ROUTINE)MainThread,(void*)this,0,&dwid);
	return hThread == INVALID_HANDLE_VALUE ? -1 : 0;
}

void* CDllHelper::GetAddr(char* funcname)
{
	return GetProcAddress(hDll, funcname);
}

bool CDllHelper::IsThreadAlive()
{
	DWORD exitcode;
	return (GetExitCodeThread(hThread, &exitcode) && exitcode == STILL_ACTIVE) ? true : false;
}

int CDllHelper::Stop()
{
	DWORD exitcode;
	if (!hThread ) return -1;
	if (GetExitCodeThread(hThread, &exitcode) && exitcode == STILL_ACTIVE ) {
		TerminateThread(hThread, 0);
		return 0;
	}
	return exitcode;
}
