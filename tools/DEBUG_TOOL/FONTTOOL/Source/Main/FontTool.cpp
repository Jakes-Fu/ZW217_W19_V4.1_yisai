// FontTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FontTool.h"
#include "FontToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class sort_string
{
public:
	bool operator()(LPCTSTR p1,LPCTSTR p2)
	{
		if(_tcscmp(p1,p2) < 0)
			return TRUE;
		else
			return FALSE;
	}
};

static const _TCHAR G_SZ_SEC_FIF[]  = _T("FontInfo");
static const  _TCHAR G_SZ_KEY_CSN[]  = _T("CharSetName");
static const  _TCHAR G_SZ_KEY_TAG[]  = _T("CharSetTag");
static const  _TCHAR G_SZ_KEY_USN[]  = _T("UnicodeSectorNum");
static const  _TCHAR G_SZ_KEY_EWF[]  = _T("EqualWidthFlag");
static const  _TCHAR G_SZ_KEY_PTS[]  = _T("PointSize");
static const  _TCHAR G_SZ_KEY_SUN[]  = _T("SpecialUniSecNum");
static const  _TCHAR G_SZ_KEY_SPT[]  = _T("SpecialPtSize");
static const  _TCHAR G_SZ_SEC_CST[]  = _T("UnicodeSector");
static const  _TCHAR G_SZ_KEY_SNO[]  = _T("%d");
static const  _TCHAR G_SZ_SEC_FNT[]  = _T("FontType");
static const  _TCHAR G_SZ_SEC_SUS[]  = _T("SpecialUnicodeSector");
static const  _TCHAR G_SZ_SEC_PSP[]  = _T("PointSizeProperty");

static const  _TCHAR G_SZ_SEC_VEC[]  = _T("VectorFont");
static const  _TCHAR G_SZ_KEY_VSP[]  = _T("ShrinkPercent");

BYTE * g_buf_ptr = NULL;
UINT   g_buf_size = 15400;
/////////////////////////////////////////////////////////////////////////////
// CFontToolApp

BEGIN_MESSAGE_MAP(CFontToolApp, CWinApp)
	//{{AFX_MSG_MAP(CFontToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontToolApp construction

CFontToolApp::CFontToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_nLogMode = 0;
	m_bSupportSubGlyf = TRUE;
	m_nBdfCalcBearingYStrategy = 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFontToolApp object

CFontToolApp g_theApp;

/////////////////////////////////////////////////////////////////////////////
// CFontToolApp initialization

BOOL CFontToolApp::InitInstance()
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

//	if(!LoadSetting())
//	{
//		return FALSE;
//	}
	CFontToolDlg dlg;
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

BOOL CFontToolApp::LoadSetting()
{
	static _TCHAR szPath[_MAX_PATH] = {0};

	DWORD dwRet = ::GetModuleFileName(NULL,szPath,_MAX_PATH);
	if(dwRet == 0)
	{
		CString str;
		str.Format(_T("Load setting configure failed: [Error:0x%08x]"),GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}

	CWaitCursor wait;


	CString strFile = szPath;
	int nFind = strFile.ReverseFind(_T('\\'));
	strFile = strFile.Left(nFind + 1);

	CString strFontToolIni = strFile +  _T("FontTool.ini");

	if(!LoadGlobalSetting(strFontToolIni))
	{
		CString str;
		str.Format(_T("Load setting configure [FontTool.ini] failed."));
		AfxMessageBox(str);
		return FALSE;
	}

	strFile += _T("*.ini");

	CFileFind fileFind;
	BOOL bFound = fileFind.FindFile(strFile);

	CStringArray aIniFile;
	std::vector<LPCTSTR> vIni;

	while(bFound)
	{
		bFound = fileFind.FindNextFile();
		CString strFilePath = fileFind.GetFilePath();
		if(strFilePath.CompareNoCase(strFontToolIni)!= 0)
		{
			aIniFile.Add(strFilePath);
			vIni.push_back((LPCTSTR)strFilePath);
		}
	}
	
	
	int nSize = aIniFile.GetSize();
	if(nSize == 0)
	{
		CString str;
		str.Format(_T("Not found char set configure file."));
		AfxMessageBox(str);
		return FALSE;
	}

	std::sort(vIni.begin(),vIni.end(),sort_string());

	_ASSERTE(vIni.size() == (UINT)nSize);

	CString strErr = _T("");

	BOOL bOK = TRUE;
	for(int i = 0; i< nSize; i++)
	{
		wait.Restore();
		if(!LoadIni(vIni[i]))
		{
			//ReleaseCSI();
			CString str;
			str.Format(_T("Load char set configure file [%s] fail.\n"),vIni[i]);
			//AfxMessageBox(str);
			//return FALSE;
			bOK = FALSE;
			strErr += str;
		}
	}

	if(!bOK)
	{
		AfxMessageBox(strErr);
	}

	if(g_buf_ptr == NULL)
	{
		g_buf_size = 15400;
		g_buf_ptr = new BYTE[g_buf_size];
		memset(g_buf_ptr,0,g_buf_size);
	}
	
	return TRUE;
}
BOOL CFontToolApp::LoadGlobalSetting(LPCTSTR lpszFile)
{
	CWaitCursor wait;
	g_buf_size = ::GetPrivateProfileInt(_T("OutlineBuffer"),_T("size"),15400,lpszFile);
	if(g_buf_size < 15400)
	{
		g_buf_size = 15400;
	}

	if(g_buf_ptr != NULL)
	{
		delete [] g_buf_ptr;
		g_buf_ptr = NULL;
	}

	g_buf_ptr = new BYTE[g_buf_size];
	if(g_buf_ptr == NULL)
	{
		AfxMessageBox(_T("Outline buffer size is too large set in FontTool.ini!"));
		return FALSE;
	}
	memset(g_buf_ptr,0,g_buf_size);

	m_nLogMode = ::GetPrivateProfileInt(_T("Log"),_T("LogMode"),0,lpszFile);

	m_nBdfCalcBearingYStrategy = ::GetPrivateProfileInt(_T("BDF"),_T("CalcBearingY"),0,lpszFile);


	_TCHAR *pszBuf = new _TCHAR[500* 50];
	memset(pszBuf,0, 500* 50* sizeof(_TCHAR));

	DWORD dwSize = GetPrivateProfileSection( _T("SubGlyph"), pszBuf, 500 * 50, lpszFile );
	if(dwSize > 0)
	{
		CStringArray agKeys;
		int nCount = EnumKeys(pszBuf,&agKeys);

		for(int i = 0; i< nCount;i++)
		{
			CString strKey = agKeys[2*i];
			CString strValue = agKeys[2*i+1];
			if(strKey.CompareNoCase(_T("support"))==0)
			{
				m_bSupportSubGlyf = _ttoi(strValue);
			}
			else
			{
				strKey.MakeLower();
				BOOL bSupport = _ttoi(strValue) == 0 ? FALSE : TRUE;
				m_mapCSSupportSubGlyfFlag.SetAt(strKey,bSupport);
			}
		}
	}

	delete [] pszBuf;

	return TRUE;

}
/*
[FontInfo]
CharSetName	     = EnglishFont
CharSetTag	     = latn
UnicodeSectorNum  = 3
EqualWidthFlag   = 0
[UnicodeSector]
;sector = unicode start, unicode end
1= 0x1000,0x2000
2= 0x5000,0x8000
3= 0xA000,0xC000
*/
BOOL CFontToolApp::LoadIni(LPCTSTR lpszFile)
{
	CWaitCursor wait;
	CHAR_SET_INFO_PTR pCSI = new CHAR_SET_INFO;

	_TCHAR szString[_MAX_PATH] = {0};
	
	//read char set name
	::GetPrivateProfileString(G_SZ_SEC_FIF,G_SZ_KEY_CSN,NULL,szString,_MAX_PATH,lpszFile);
	_tcscpy(pCSI->szCharSetName,szString);

	//read char set name
	::GetPrivateProfileString(G_SZ_SEC_FIF,G_SZ_KEY_TAG,NULL,szString,_MAX_PATH,lpszFile);
	char szTmp[5]= {0};
	WideCharToMultiByte(CP_ACP,0,szString,_tcslen(szString)>4?4:_tcslen(szString),szTmp,5,NULL,NULL);
	memcpy(pCSI->tag,szTmp,sizeof(pCSI->tag));

	CString strTag;
	strTag.Format(_T("%c%c%c%c"),pCSI->tag[0],pCSI->tag[1],pCSI->tag[2],pCSI->tag[3]);
	pCSI->bSupportSubGlyph = IsSupportSubGlyph(strTag);
	
	//read Unicode Sector Num
	memset(szString,0,sizeof(szString));
	pCSI->nSectorNum = ::GetPrivateProfileInt(G_SZ_SEC_FIF,G_SZ_KEY_USN,0,lpszFile);

	//read Equal Width Flag 
	pCSI->nWidthEqualFlag = ::GetPrivateProfileInt(G_SZ_SEC_FIF,G_SZ_KEY_EWF,0,lpszFile);

	//read point size 
	::GetPrivateProfileString(G_SZ_SEC_FIF,G_SZ_KEY_PTS,NULL,szString,_MAX_PATH,lpszFile);
	if(_tcslen(szString) ==  0)
	{
		pCSI->nFtTblNum = 0;
		pCSI->pFtTblSize = NULL;
	}
	else
	{
		CUIntArray agPtSize;
		LPTSTR lpBuf = szString;
		LPTSTR  lpFind = _tcschr(lpBuf, _T(','));
		while(lpFind != NULL)
		{
			*lpFind = _T('\0');

			agPtSize.Add(_ttoi(lpBuf));
			lpBuf = lpFind + 1;
			lpFind = _tcschr(lpBuf, _T(','));
		}
		agPtSize.Add(_ttoi(lpBuf));
		int nCount = agPtSize.GetSize();
		if(nCount!=0)
		{
			pCSI->nFtTblNum = nCount;
			pCSI->pFtTblSize = new int[nCount];
			for(int i=0;i<nCount;i++)
			{
				pCSI->pFtTblSize[i] = agPtSize[i];
			}
		}
	}

	if(pCSI->nSectorNum != 0)
	{
		pCSI->pUCBoundTable = new UC_BOUND_T[pCSI->nSectorNum];
		memset(pCSI->pUCBoundTable,0,sizeof(UC_BOUND_T) * pCSI->nSectorNum);

		CString strIDKey;
		_TCHAR * pszBuf = new _TCHAR[pCSI->nSectorNum * 50];
		memset(pszBuf,0, pCSI->nSectorNum * sizeof(_TCHAR)*50);

		DWORD dwSize = GetPrivateProfileSection( G_SZ_SEC_CST, pszBuf, pCSI->nSectorNum * 50, lpszFile );

		if(dwSize > 0)
		{
			CStringArray aKeyVale;
			
			int nRealCount = EnumKeys(pszBuf,&aKeyVale);
			
			if(nRealCount != pCSI->nSectorNum)
			{
				DeleteCSI(pCSI);
				delete [] pszBuf;
				return FALSE;
			}
			
			MSG msg;
			::PostMessage(m_pMainWnd->m_hWnd,WM_PROGRESS,PROG_BEGIN,pCSI->nCharCount);
			for(int i =0; i< pCSI->nSectorNum; i++)
			{
				while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);	
				}	
				::PostMessage(m_pMainWnd->m_hWnd,WM_PROGRESS,PROG_PROCEED,i+1);
				
				wait.Restore();
				
				CString strTemp = aKeyVale.GetAt(2*i+1);
				_stscanf(strTemp,_T("0x%X,0x%X"),
					&pCSI->pUCBoundTable[i].dwUCStart, &pCSI->pUCBoundTable[i].dwUCEnd);	
				
				pCSI->nCharCount += (pCSI->pUCBoundTable[i].dwUCEnd - pCSI->pUCBoundTable[i].dwUCStart + 1);
			}
			
			::PostMessage(m_pMainWnd->m_hWnd,WM_PROGRESS,PROG_END,100);
		}

		delete [] pszBuf;

	}

//////////////////////////////////////////////////////////////////////////

	//read External Unicode Sector Num
	memset(szString,0,sizeof(szString));
	pCSI->nExSectorNum = ::GetPrivateProfileInt(G_SZ_SEC_FIF,G_SZ_KEY_SUN,0,lpszFile);

	//read point size 
	::GetPrivateProfileString(G_SZ_SEC_FIF,G_SZ_KEY_SPT,NULL,szString,_MAX_PATH,lpszFile);
	if(_tcslen(szString) ==  0)
	{
		pCSI->nExFtTblNum = 0;
		pCSI->pExFtTblSize = NULL;
	}
	else
	{
		CUIntArray agPtSize;
		LPTSTR lpBuf = szString;
		LPTSTR  lpFind = _tcschr(lpBuf, _T(','));
		while(lpFind != NULL)
		{
			*lpFind = _T('\0');

			agPtSize.Add(_ttoi(lpBuf));
			lpBuf = lpFind + 1;
			lpFind = _tcschr(lpBuf, _T(','));
		}
		agPtSize.Add(_ttoi(lpBuf));
		int nCount = agPtSize.GetSize();
		if(nCount!=0)
		{
			pCSI->nExFtTblNum = nCount;
			pCSI->pExFtTblSize = new int[nCount];
			for(int i=0;i<nCount;i++)
			{
				pCSI->pExFtTblSize[i] = agPtSize[i];
			}
		}
	}

	if(pCSI->nExSectorNum !=  0)
	{
		pCSI->pExUCBoundTable = new UC_BOUND_T[pCSI->nExSectorNum];
		memset(pCSI->pExUCBoundTable,0,sizeof(UC_BOUND_T) * pCSI->nExSectorNum);

		CString strIDKey;
		_TCHAR *pszBuf = new _TCHAR[pCSI->nExSectorNum * 50];
		memset(pszBuf,0, pCSI->nExSectorNum * sizeof(_TCHAR)* 50);

		DWORD dwSize = GetPrivateProfileSection( G_SZ_SEC_SUS, pszBuf, pCSI->nExSectorNum * 50, lpszFile );

		if(dwSize > 0)
		{
			CStringArray aKeyVale;
			
			int nRealCount = EnumKeys(pszBuf,&aKeyVale);
			
			if(nRealCount != pCSI->nExSectorNum)
			{
				DeleteCSI(pCSI);
				delete [] pszBuf;
				return FALSE;
			}
			
			for(int i =0; i< pCSI->nExSectorNum; i++)
			{
				CString strTemp = aKeyVale.GetAt(2*i+1);	
				_stscanf(strTemp,_T("0x%X,0x%X"),
					&pCSI->pExUCBoundTable[i].dwUCStart, &pCSI->pExUCBoundTable[i].dwUCEnd);			
			}
		}

		delete [] pszBuf;


	}

	// for shrink size
	{
		CString strIDKey;
		_TCHAR *pszBuf = new _TCHAR[255* 50];
		memset(pszBuf,0, 255* 50* sizeof(_TCHAR));

		DWORD dwSize = GetPrivateProfileSection( G_SZ_SEC_PSP, pszBuf, 255 * 50, lpszFile );
		pCSI->pmapFTProp = new MAP_FTPROP;

		if(dwSize > 0)
		{
			CStringArray aKeyVale;
			int nRealCount = EnumKeys(pszBuf,&aKeyVale);			
			int nKey=0;
			int nValue1=0;
			int nValue2=0;

			for(int i =0; i< nRealCount; i++)
			{
				FONT_PROPERTY_PTR pProp = new FONT_PROPERTY_T;
				CString strTemp = aKeyVale.GetAt(2*i);
				_stscanf(strTemp,_T("%d"),&nKey);
				pProp->nFontSize = nKey;

				strTemp = aKeyVale.GetAt(2*i+1);		
				_stscanf(strTemp,_T("%d,%d"),&nValue1,&nValue2);
				pProp->nFontType = nValue1;
				pProp->nShrinkSize = nValue2;

				pCSI->pmapFTProp->SetAt(nKey,pProp);				
			}
		}

		delete [] pszBuf;
	}

	pCSI->nVectorShrinkPercent = ::GetPrivateProfileInt(G_SZ_SEC_VEC,G_SZ_KEY_VSP,100,lpszFile);
	if(pCSI->nVectorShrinkPercent < 50 || pCSI->nVectorShrinkPercent >100)
	{
		pCSI->nVectorShrinkPercent = 100;
	}

	//	TRACE(_T("CharSetName: %s\n"),pCSI->szCharSetName);
	CHAR_SET_INFO_PTR pTmpCSI = NULL;
	if(m_mapCSI.Lookup(pCSI->szCharSetName,pTmpCSI))
	{
		_TCHAR sz[100] = {0};
		_stprintf(sz,_T("-%d"),m_mapCSI.GetCount());
		_tcscat(pCSI->szCharSetName,sz);
	}

	m_mapCSI.SetAt(pCSI->szCharSetName,pCSI);
	m_aCharSetName.Add(pCSI->szCharSetName);

	return TRUE;
}


int CFontToolApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	ReleaseCSI();
	if(g_buf_ptr != NULL)
	{
		delete [] g_buf_ptr;
		g_buf_ptr = NULL;
	}
	g_buf_size = 0;

	return CWinApp::ExitInstance();
}

void CFontToolApp::ReleaseCSI()
{
	m_aCharSetName.RemoveAll();

	POSITION pos = m_mapCSI.GetStartPosition();

	CHAR_SET_INFO_PTR pCSI = NULL;
	CString strKey;
	while(pos)
	{
		m_mapCSI.GetNextAssoc(pos,strKey,pCSI);
		DeleteCSI(pCSI);
		pCSI = NULL;
	}

	m_mapCSI.RemoveAll();
}

int CFontToolApp::EnumKeys(LPCTSTR pSection,CStringArray* pKeys)
{
    ASSERT(pSection != NULL);
    ASSERT(pKeys != NULL);
	
    int number = 0;
    while(1)
    {
        CString strLine = pSection;
        int nLen = strLine.GetLength();
        if(0 == nLen)
        {
            // All keys are read
            break;
        }
        
        int nIndex = strLine.Find(_T('='));
        if(-1 == nIndex)
        {
            // Not a valid key
            continue;
        }
        CString strKey = strLine.Left(nIndex);
        strKey.TrimLeft();
        strKey.TrimRight();
        pKeys->Add(strKey);
		
        CString strData = strLine.Right(nLen - nIndex - 1);
        strData.TrimLeft();
        strData.TrimRight();
        pKeys->Add(strData);
		
        number++;
		
        pSection += nLen + 1;
		
    }
	
    return number;
}
BOOL CFontToolApp::IsSupportSubGlyph(LPCTSTR pszCSTag)
{
	if(pszCSTag == NULL)
		return FALSE;

	if(m_bSupportSubGlyf)
		return TRUE;

	CString strTag = pszCSTag;
	strTag.MakeLower();
	UINT bSuppprt = FALSE;
	if(m_mapCSSupportSubGlyfFlag.Lookup(strTag,bSuppprt))
	{
		return bSuppprt;
	}

	return FALSE;
}

void CFontToolApp::DeleteCSI(CHAR_SET_INFO_PTR pCSI)
{
	if(pCSI != NULL)
	{
		SAFE_FREE_ARRAY(pCSI->pUCBoundTable);
		SAFE_FREE_ARRAY(pCSI->pFtTblSize);
		SAFE_FREE_ARRAY(pCSI->pExUCBoundTable);
		SAFE_FREE_ARRAY(pCSI->pExFtTblSize);

		if(pCSI->pmapFTProp != NULL)
		{
			int nKey;
			FONT_PROPERTY_PTR pProp = NULL;
			POSITION posFTP = pCSI->pmapFTProp->GetStartPosition();
			while(posFTP)
			{
				pCSI->pmapFTProp->GetNextAssoc(posFTP,nKey,pProp);
				if(pProp != NULL)
					delete pProp;
			}	
			pCSI->pmapFTProp->RemoveAll();
			delete pCSI->pmapFTProp;
			pCSI->pmapFTProp = NULL;

		}
		delete pCSI;
	}
}
