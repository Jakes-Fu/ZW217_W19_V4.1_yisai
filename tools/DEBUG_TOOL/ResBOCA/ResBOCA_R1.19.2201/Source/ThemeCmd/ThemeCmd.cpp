// ThemeCmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ThemeCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_LEN  (4096)

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

void ShowUsage()
{
	_tprintf(_T("Please input parameter: -def <\"xxx.bin\"> -dir <\"xxx\"> -out <\"xxx.bin\"> -maxsize <\"n\">\n\n"));
	_tprintf(_T("-def <\"xxx.xrp\">      --default theme file path\n"));
	_tprintf(_T("-dir <\"xxx\">		     --theme resource folder path\n"));
	_tprintf(_T("-out <\"xxx.bin\">      --new theme bin file path\n"));
	_tprintf(_T("-maxsize <\"n\">        --max size of theme bin file, K unit\n"));
	_tprintf(_T("\r\n"));
}

UINT ReadProc(LPVOID lpVoid)
{
	HANDLE hRead = (HANDLE)(lpVoid);
	TCHAR buffer[MAX_LEN] ={0};
	DWORD bytesRead;
	while(true)
	{
		if(!ReadFile(hRead,buffer,(MAX_LEN-1)*sizeof(TCHAR),&bytesRead,NULL))
			break;
		//cout<< buffer << endl;
		
		// 		if(_tcsstr(buffer, _T("OK")) != NULL)
		// 		{
		// 			DWORD dwExitCode;
		// 			GetExitCodeProcess(pi.hProcess,&dwExitCode);
		// 			ExitProcess(dwExitCode);
		// 		}
		if(bytesRead>0)
		{
			_tprintf(buffer);
			//_tprintf(_T("\n"));
			memset(buffer,0,MAX_LEN*sizeof(TCHAR));
		}
		//Sleep(2000);
		
	}

	return 1;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	UNUSED_ALWAYS(envp);
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf( _T("Fatal Error: MFC initialization failed\n"));
		return 1;
	}
		
    CString strDefPath = _T("");
	CString strDirPath = _T("");
    CString strOutPath = _T("");
	DWORD dwMaxSize = MAXDWORD;
	for(int i = 1; i< argc; i++)
	{		
		if(_tcsicmp(argv[i],_T("-def")) == 0)
		{
			if( (i+1) < argc )
			{				
				strDefPath = argv[++i];					
			}
			else
			{
				_tprintf(_T("parameters error -def.\n\n"));
				ShowUsage();
				return 1;
			}
		}
		else if(_tcsicmp(argv[i],_T("-dir")) == 0)
		{
			if( (i+1) < argc )
			{				
				strDirPath = argv[++i];				
			}	
			else
			{
				_tprintf(_T("parameters error -dir.\n\n"));
				ShowUsage();
				return 1;
			}
		}
		else if(_tcsicmp(argv[i],_T("-out")) == 0)
		{
			if( (i+1) < argc )
			{				
				strOutPath = argv[++i];				
			}	
			else
			{
				_tprintf(_T("parameters error -out.\n\n"));
				ShowUsage();
				return 1;
			}
		}
		else if(_tcsicmp(argv[i],_T("-maxsize")) == 0)
		{
			if( (i+1) < argc )
			{				
				dwMaxSize = _ttol(argv[++i]);
				if(dwMaxSize == 0)
				{
					_tprintf(_T("parameters error -maxsize, maxsize is zero.\n\n"));
					ShowUsage();
					return 1;
				}
			}	
			else
			{
				_tprintf(_T("parameters error -maxsize.\n\n"));
				ShowUsage();
				return 1;
			}
		}
	}

	if(strDefPath.IsEmpty() || strDirPath.IsEmpty() || strOutPath.IsEmpty()) 
	{	
		if(strDefPath.IsEmpty())
		{
			_tprintf(_T("def file path is empty\n"));
		}
		if(strDirPath.IsEmpty())
		{	
			_tprintf(_T("dir folder path is empty\n"));
		}
		if(strOutPath.IsEmpty())
		{	
			_tprintf(_T("out file path is empty\n"));
		}
	 
		ShowUsage();
		return 1;
	}

	
	TCHAR szParamLine[512*4] = {0};	

	if(MAXDWORD == dwMaxSize)
	{
		_stprintf(szParamLine,_T("ResOver -def \"%s\" -dir \"%s\" -out \"%s\""),strDefPath,strDirPath,strOutPath);
	}
	else
	{
		_stprintf(szParamLine,_T("ResOver -def \"%s\" -dir \"%s\" -out \"%s\" -maxsize %ud"),strDefPath,strDirPath,strOutPath,dwMaxSize);
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

	AfxBeginThread(ReadProc,(LPVOID)hStdRead);
	
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
	
	if(!::CreateProcess(_T("ResOver.exe"),szParamLine,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
	{
		_tprintf(_T("Create ResOver.exe Process Failed\n"));
		return 1;
	}
	
	CloseHandle(hStdWrite);

	_tprintf(_T("run...\n"));
//    _tprintf(_T("-"));	

    DWORD dwExitCode = 0;
// 	int nSign=0;
	while(GetExitCodeProcess(pi.hProcess,&dwExitCode))
	{
		if(dwExitCode != STILL_ACTIVE)
		{
			break;
		}
// 		nSign ++;
// 		int nCase = nSign%4;
// 		switch(nCase)
// 		{
// 		case 0:
// 			_tprintf(_T("\b"));
// 			_tprintf(_T("-"));
// 			break;
// 		case 1:
// 			_tprintf(_T("\b"));
// 			_tprintf(_T("\\"));
// 			break;
// 		case 2:
// 			_tprintf(_T("\b"));
// 			_tprintf(_T("|"));
// 		    break;
// 		case 3:
// 			_tprintf(_T("\b"));
// 			_tprintf(_T("/"));
// 		    break;
// 		default:
// 		    break;
// 		}
		Sleep(100);
				
	}

	return nRetCode;


	
}




