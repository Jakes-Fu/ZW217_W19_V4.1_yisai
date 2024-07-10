// ResOver.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ResOver.h"
#include "ResOverDlg.h"
#include "ResParser.h"
#include "CmdNtac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResOverApp

BEGIN_MESSAGE_MAP(CResOverApp, CWinApp)
	//{{AFX_MSG_MAP(CResOverApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResOverApp construction

CResOverApp::CResOverApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pGdiToken = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CResOverApp object

CResOverApp g_theApp;

/////////////////////////////////////////////////////////////////////////////
// CResOverApp initialization

BOOL CResOverApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

	CString strDefPath = _T("");
	CString strDirPath = _T("");
    CString strOutPath = _T("");
	DWORD   dwMaxSize = MAXDWORD;
	BOOL bNTACMode = FALSE;
	NTAC_Args_T tNtacArg;
	for (int i = 1; i < __argc; i++)
	{		
		if (i == 1 && 0 == _tcsicmp(__targv[i],_T("-NTAC")))
		{
			bNTACMode = TRUE;
			continue;
		}
		if (bNTACMode)
		{
			if (0 == _tcsicmp(__targv[i],_T("-import")))
			{
				if((i+1)<__argc)
				{
					_tcsncpy(tNtacArg.szInFile,__targv[++i],MAX_PATH);	
					tNtacArg.eInType = E_EXECL;
					if (0 == GetFileExt(tNtacArg.szInFile).CompareNoCase(_T("ntac")))
					{
						tNtacArg.eInType = E_NTAC;
					}
					
				}

			}
			else if (0 == _tcsicmp(__targv[i],_T("-export")))
			{
				if((i+1)<__argc)
				{
					_tcsncpy(tNtacArg.szOutFile,__targv[++i],MAX_PATH);	
					tNtacArg.eInType = E_EXECL;
					if (0 == GetFileExt(tNtacArg.szOutFile).CompareNoCase(_T("ntac")))
					{
						tNtacArg.eOutType = E_NTAC;
					}
				}

			}
		}
		else
		{
			if(_tcsicmp(__targv[i],_T("-def")) == 0)
			{
				if( (i+1) < __argc )
				{				
					strDefPath = __targv[++i];					
				}
				else
				{
					_tprintf(_T("parameters error -def.\n\n"));	
					return 1;
				}
			}
			else if(_tcsicmp(__targv[i],_T("-dir")) == 0)
			{
				if( (i+1) < __argc )
				{				
					strDirPath = __targv[++i];				
				}	
				else
				{
					_tprintf(_T("parameters error -dir.\n\n"));	
					return FALSE;
				}
			}
			else if(_tcsicmp(__targv[i],_T("-out")) == 0)
			{
				if( (i+1) < __argc )
				{				
					strOutPath = __targv[++i];				
				}	
				else
				{
					_tprintf(_T("parameters error -out.\n\n"));		
					return FALSE;
				}
			}
			else if(_tcsicmp(__targv[i],_T("-maxsize")) == 0)
			{
				if( (i+1) < __argc )
				{				
					dwMaxSize = _ttol(__targv[++i]);
					dwMaxSize *= 1024;
					if(dwMaxSize == 0)
					{
						_tprintf(_T("parameters error -maxsize, maxsize is zero.\n\n"));		
						return FALSE;
					}
				}	
				else
				{
					_tprintf(_T("parameters error -maxsize.\n\n"));		
					return FALSE;
				}
			}
		}
	}

	if(__argc > 1)
	{
		if (bNTACMode)
		{
			if ( 0 == _tcslen(tNtacArg.szInFile) || 0 == _tcslen(tNtacArg.szOutFile))
			{
				_tprintf(_T("Invalid parameters\n"));		
				return TRUE;
			}
			CCmdNtac cCmdNtac;
			return cCmdNtac.GenNtac(tNtacArg)? FALSE : TRUE;
		}
		else
		{
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

				return TRUE;
			}
			else
			{
				CResParser rp;
				rp.SetCmdParam(dwMaxSize);
				if(rp.LoadResFile(strDefPath))
				{
					if(rp.LoadThmFolder(strDirPath))
					{
						_tprintf(_T("Replace theme resource file success.\n\n"));
					}
					else
					{
						_tprintf(_T("Replace theme resource file failed.\n\n"));
						return TRUE;
					}

					if(rp.SaveResFile(strOutPath))
					{
						_tprintf(_T("Generate theme resource file success.\n\n"));
					}
					else
					{
						_tprintf(_T("\n\nGenerate theme resource file failed.\n\n"));
					}

					return TRUE;
				}
				else
				{
					_tprintf(_T("Load default theme file [%s] failed.\n\n"),strDefPath);	
					return TRUE;
				}
			}
		}
		
	}


	CResOverDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CResOverApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	GdiplusShutdown(m_pGdiToken);
	return CWinApp::ExitInstance();
}

BOOL CResOverApp::DeleteDirectory(LPCTSTR lpszDirName)// DeleteDirectory(_T("c:\\aaa"))
{
	CFileFind tempFind;
	BOOL IsFinded = TRUE;
	IsFinded = (BOOL)tempFind.FindFile(lpszDirName);
	if(!IsFinded)
	{
		tempFind.Close();
		return TRUE;
	}
	tempFind.Close();
	
	DWORD dwAtt=0;
	
    _TCHAR tempFileFind[MAX_PATH] = {0};
	_stprintf(tempFileFind,_T("%s\\*.*"),lpszDirName);
    IsFinded=(BOOL)tempFind.FindFile(tempFileFind);
    while(IsFinded)
    {
        IsFinded=(BOOL)tempFind.FindNextFile();
        if(!tempFind.IsDots())
        {
            _TCHAR foundFileName[MAX_PATH] = {0};
            _tcscpy(foundFileName,tempFind.GetFileName().GetBuffer(MAX_PATH));
            if(tempFind.IsDirectory())
            {
                _TCHAR tempDir[MAX_PATH] = {0};
                _stprintf(tempDir,_T("%s\\%s"),lpszDirName,foundFileName);								
				
                DeleteDirectory(tempDir);
            }
            else
            {
                _TCHAR tempFileName[MAX_PATH] = {0};
                _stprintf(tempFileName,_T("%s\\%s"),lpszDirName,foundFileName);
				
				dwAtt = 0;						
				dwAtt	= ::GetFileAttributes(tempFileName);
				dwAtt &= ~FILE_ATTRIBUTE_READONLY;
				::SetFileAttributes(tempFileName,dwAtt);						
				
                DeleteFile(tempFileName);
            }
        }
    }
    tempFind.Close();
	dwAtt = 0;						
	dwAtt	= ::GetFileAttributes(lpszDirName);
	dwAtt &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(lpszDirName,dwAtt);	
	
    if(!RemoveDirectory(lpszDirName))
    {     
        return FALSE;
    }
    return TRUE;
}
BOOL CResOverApp::CreateDeepDirectory(LPCTSTR lpszDirName)
{
	_ASSERTE(lpszDirName != NULL);
	CStringArray aDir;

	CString strDirPath = lpszDirName;
	CString strTmp;
	int nFind = -1;
	aDir.Add(strDirPath);
	nFind = strDirPath.ReverseFind(_T('\\'));
	while(nFind > 2) // [D:\]aaa\bbb
	{
		strDirPath = strDirPath.Left(nFind );			
		aDir.Add(strDirPath);
		nFind = strDirPath.ReverseFind(_T('\\'));
	}
	
	
	CFileFind finder;
	for(int i= aDir.GetSize()-1; i>=0; i--)
	{
		if(!CreateDirectory(aDir.GetAt(i),NULL))
		{
			CString str = aDir.GetAt(i);
			if( GetLastError() != ERROR_ALREADY_EXISTS )
			{
				_ASSERTE( 0 );
				return FALSE;
			}
		}		
	}
	
	return TRUE;
}
///////////////////////////////////////////////////////
DWORD CResOverApp::Color565To24( DWORD dwColor )
{
// 	WORD wRMask   = (WORD)(bIs555 ? 0x7C00 : 0xF800);
// 	int  nRRShift = bIs555 ? 10     : 11;
// 	int  nRLShift = 3;
// 	WORD wGMask   = (WORD)(bIs555 ? 0x03E0 : 0x07E0);
// 	int  nGRShift = 5;
// 	int  nGLShift = bIs555 ? 3 : 2;
// 	WORD wBMask   = 0x001F;
// 	int  nBLShift = 3;

	WORD wRMask   = (WORD)(0xF800);
	int  nRRShift = 11;
	int  nRLShift = 3;
	WORD wGMask   = (WORD)(0x07E0);
	int  nGRShift = 5;
	int  nGLShift = 2;
	WORD wBMask   = (WORD)0x001F;
	int  nBLShift = 3;


	BYTE b = (BYTE)(  (wBMask & dwColor)              << nBLShift);
	BYTE g = (BYTE)( ((wGMask & dwColor) >> nGRShift) << nGLShift);
	BYTE r = (BYTE)( ((wRMask & dwColor) >> nRRShift) << nRLShift);

	return RGB(r,g,b);
   
}
DWORD CResOverApp::Color24To565(DWORD dwColor)
{
	DWORD dwColor565 = 0;
	WORD wValue = 0;
	BYTE r = GetRValue(dwColor);
	BYTE g = GetGValue(dwColor);
	BYTE b = GetBValue(dwColor);
	
	wValue = (WORD)(b >> 3);
    dwColor565  = wValue;


	wValue = (WORD)(g >> 2);
	wValue <<= 5;
	dwColor565 |= wValue;

	wValue = (WORD)(r >> 3);
	wValue <<= 11;
	dwColor565 |= wValue;

	return dwColor565;
}