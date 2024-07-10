// BTACmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h> 
#include <process.h>
#include "Afxwin.h"

void ReadProc(void * lpVoid)
{
	HANDLE hRead = (HANDLE)(lpVoid);
	char buffer[4096] ={0};
	DWORD bytesRead;
	while(true)
	{
		if(!ReadFile(hRead,buffer,4095,&bytesRead,NULL))
			break;		
		printf(buffer);	
	}

	_endthread();
}

void Usage3DTable(bool bHead)
{
	if(bHead)
	{
		_tprintf(_T("Usage:\n"));
	}
	_tprintf(_T("\tBTACmd 3dtable [Slide Width] [Slide Height] [Desktop Width] [Desktop Height] [Output Path]\n"));
}
void UsageB2A(bool bHead)
{
	if(bHead)
	{
		_tprintf(_T("Usage:\n"));
	}
	_tprintf(_T("\tBTACmd b2a  <-b> <bmp file path> <array file path> [-l]\n"));
	_tprintf(_T("\tBTACmd b2a  <-f> <bmp folder path> <array file path> [-l]\n"));
	_tprintf(_T("\tBTACmd b2a  <-a> <bmp anim folder path> <array file path> [-l]\n"));
	_tprintf(_T("\tBTACmd b2a  <-j> <jpg file path> <array file path> [-l]\n"));
	_tprintf(_T("\tBTACmd b2a  <-jf> <jpg folder path> <array file path> [-l]\n"));
	_tprintf(_T("\tBTACmd b2a  <-g> <gif file path> <array file path> [-l]\n"));
	_tprintf(_T("\tBTACmd b2a  <-gf> <gif folder path> <array file path> [-l]\n"));
	_tprintf(_T("\tBTACmd b2a  <-p> <png file path> <array file path> [-l]\n"));
	_tprintf(_T("\tBTACmd b2a  <-pf> <png folder path> <array file path> [-l]\n"));
}

void Usage()
{
	Usage3DTable(true);
	UsageB2A(false);
}


int main(int argc, char* argv[])
{
	int nRetCode = 0;
	
	TCHAR szParam[2048] = {0};

	if(argc < 5)
	{
		Usage();
		return 1;
	}
	
	if(_tcsicmp(argv[1], _T("3dtable")) == 0)
	{
		if( argc < 7)
		{
			Usage3DTable(true);
			return 1;
		}

		_stprintf(szParam, _T("BmpToArray -%s -%s -%s -%s -%s -\"%s\""), argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
	}
	else if(_tcsicmp(argv[1], _T("b2a")) == 0)
	{
		if( argc < 5)
		{
			UsageB2A(true);
			return 1;
		}
		if(_tcsicmp(argv[2], _T("-b")) != 0 &&
		   _tcsicmp(argv[2], _T("-f")) != 0 && 
		   _tcsicmp(argv[2], _T("-a")) != 0 && 
		   _tcsicmp(argv[2], _T("-j")) != 0 && 
		   _tcsicmp(argv[2], _T("-jf")) != 0 && 
		   _tcsicmp(argv[2], _T("-g")) != 0 && 
		   _tcsicmp(argv[2], _T("-gf")) != 0 &&
		   _tcsicmp(argv[2], _T("-p")) != 0 && 
		   _tcsicmp(argv[2], _T("-pf")) != 0 )
		{
			UsageB2A(true);
			return 1;
		}

		
		if(argc == 6)
		{
			_stprintf(szParam, _T("BmpToArray -%s %s \"%s\" \"%s\" -l"), argv[1], argv[2], argv[3], argv[4]);
		}
		else
		{
			_stprintf(szParam, _T("BmpToArray -%s %s \"%s\" \"%s\""), argv[1], argv[2], argv[3], argv[4]);
		}
	}
	else
	{
		Usage();
		return 1;
	}

	

	HANDLE hStdRead,hStdWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	if(!CreatePipe(&hStdRead,&hStdWrite,&sa,0))
	{	
		_tprintf(_T("Create Pipe Failed\n"));		
		return 1;
	}

	_beginthread(ReadProc, 0, (void *)hStdRead);
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hStdWrite;
	si.hStdOutput = hStdWrite;
	//	si.hStdInput  = hStdRead;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	TCHAR szModulePath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szModulePath,MAX_PATH);
	CString strModuleName = szModulePath;
	CString strCurDir = szModulePath;
	int nFind = strCurDir.ReverseFind(_T('\\'));
	strCurDir = strCurDir.Left(nFind);
	::SetCurrentDirectory(strCurDir);
	
	if(!::CreateProcess(_T("BmpToArray.exe"),szParam,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
	{
		DWORD dwErr = GetLastError();

		printf("Create BmpToArray Failed\n");
		return 1;
	}
	
	CloseHandle(hStdWrite);

	_tprintf(_T("run...\n"));
//   _tprintf(_T("-"));	

    DWORD dwExitCode = 0;
//	int nSign=0;
	while(GetExitCodeProcess(pi.hProcess,&dwExitCode))
	{
		if(dwExitCode != STILL_ACTIVE)
		{
			break;
		}
/*		nSign ++;
		int nCase = nSign%4;
		switch(nCase)
		{
		case 0:
			_tprintf(_T("\b"));
			_tprintf(_T("-"));
			break;
		case 1:
			_tprintf(_T("\b"));
			_tprintf(_T("\\"));
			break;
		case 2:
			_tprintf(_T("\b"));
			_tprintf(_T("|"));
		    break;
		case 3:
			_tprintf(_T("\b"));
			_tprintf(_T("/"));
		    break;
		default:
		    break;
		}
*/
		Sleep(500);
				
	}

	return nRetCode;

}
