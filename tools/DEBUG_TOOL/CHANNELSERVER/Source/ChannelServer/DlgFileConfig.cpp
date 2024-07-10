// DlgFileConfig.cpp : implementation file
//

#include "stdafx.h"
#include "channelserver.h"
#include "DlgFileConfig.h"
#include "Function.h"
#include "DiagChan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFileConfig dialog
#define CHANNEL_FILE_NAME		_T("\\ChannelFiles.log")

CDlgFileConfig::CDlgFileConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFileConfig)
	m_strTmpFiles = _T("");
	m_nTmpFlowType = 0;
	//}}AFX_DATA_INIT
	m_nFlowType = 0;
	m_strFiles = _T("");
	m_nEndian = ENDIAN_LITTLE;

	TCHAR szChannelFile[ MAX_PATH ];
	GetAppPath( szChannelFile );
    _tcscat( szChannelFile, CHANNEL_FILE_NAME );
	m_strMergedFile = szChannelFile;
}


void CDlgFileConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFileConfig)
	DDX_Text(pDX, IDC_FC_EDT_FILE, m_strTmpFiles);
	DDX_Radio(pDX, IDC_FC_RDO_DIAG, m_nTmpFlowType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFileConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgFileConfig)
	ON_BN_CLICKED(IDC_FC_BTN_BRW, OnBtnBrw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFileConfig message handlers

void CDlgFileConfig::OnBtnBrw() 
{
	// TODO: Add your control notification handler code here
	TCHAR    strFilter[] = _T( "log file (*.log;)|*log|All files(*.*)|*.*||" );
	TCHAR    strCaption[]= _T( "Open" );
	
	// Get the path name  
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY| OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT ,
		strFilter);
	
	dlg.m_ofn.lpstrTitle = strCaption;
	
	if (dlg.DoModal() != IDOK)
		return;	
	
	UpdateData(TRUE);

	m_strTmpFiles.TrimLeft();
	m_strTmpFiles.TrimRight();

	if(!m_strTmpFiles.IsEmpty())
	{
		if(AfxMessageBox(_T("Do you want to empty the files selected before?"),MB_YESNO) == IDYES)
		{
			m_strTmpFiles.Empty();
		}
	}

	POSITION pos = dlg.GetStartPosition();
	while( pos != NULL )
	{
		m_strTmpFiles += dlg.GetNextPathName( pos);
		m_strTmpFiles += _T("|");
	}
	
	UpdateData( FALSE );
}

void CDlgFileConfig::LoadConfig(LPCTSTR pszCfgFile)
{
	_TCHAR szTmp[MAX_PATH*100] = {0};
	
	CString strSec = _T("FILE");
	
	m_nFlowType = GetPrivateProfileInt( strSec, _T("FlowType"), 0, pszCfgFile );	
	GetPrivateProfileString( strSec, _T("Files"), _T(""), szTmp, MAX_PATH, pszCfgFile );
	m_strFiles = szTmp;
}

void CDlgFileConfig::SaveConfig(LPCTSTR pszCfgFile)
{
	CString strSec = _T("FILE");

	CString strTmp;
	strTmp.Format(_T("%d"),m_nFlowType);
	WritePrivateProfileString(strSec, _T("FlowType"),strTmp,pszCfgFile);
	WritePrivateProfileString(strSec, _T("Files"),m_strFiles,pszCfgFile);
}

BOOL CDlgFileConfig::CheckConfig()
{
	UpdateData();
	m_strTmpFiles.TrimLeft();
	m_strTmpFiles.TrimRight();
	if(m_strTmpFiles.IsEmpty())
	{
		AfxMessageBox( _T("Input Error: File path is empty!"), MB_OK | MB_ICONERROR );
		return FALSE;
	}
	return TRUE;
}

void CDlgFileConfig::UpdateConfig()
{
	UpdateData();
	m_strFiles = m_strTmpFiles;
	m_nFlowType = m_nTmpFlowType;
}

BOOL CDlgFileConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nTmpFlowType = m_nFlowType;
	m_strTmpFiles = m_strFiles;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LPCTSTR CDlgFileConfig::GetCurFile(int nIndex)
{
	m_strCurFile = _T("");

	CStringArray agFiles;
	int nCount = SplitStr(m_strFiles, agFiles,_T('|'));
	CFileFind finder;

	for(int i=0; i<nCount; i++)
	{
		if(!finder.FindFile(agFiles[i]))
		{
			agFiles.RemoveAt(i);
			i--;
			nCount--;
		}
	}

	if(nIndex < 0 || nIndex >= nCount)
	{
		return m_strCurFile;
	}

	m_strCurFile = agFiles[nIndex];	

	return m_strCurFile;
}

BOOL CDlgFileConfig::MergeChannelFiles()
{
	FILE* pChannelFile = NULL;	
	CString strFiles = m_strFiles;
	TCHAR* lpszFile = wcstok( strFiles.GetBuffer( strFiles.GetLength()), _T("|") );
	if( lpszFile == NULL)
		return FALSE;
	BOOL bSuccess = CopyFile( lpszFile, m_strMergedFile, FALSE );
	if( !bSuccess )
		return FALSE;
	lpszFile = wcstok( NULL, _T( "|" ) );
	if( lpszFile != NULL )
	{
		pChannelFile = _wfopen( m_strMergedFile,  _T("ab+") );
		if( pChannelFile == NULL )
			return FALSE;
	}
	while( lpszFile != NULL )
	{
		FILE* pFile = _wfopen( lpszFile, _T("rb") );
		if( pFile != NULL )
		{
			BYTE buff[ 0x10000 ]={0};
			int nReadCount = 0;
			fseek( pFile, 0,  SEEK_SET );
			while( !feof( pFile ))
			{
				ZeroMemory( buff, sizeof( buff ) );
				nReadCount = ::fread( buff, 1, sizeof( buff ), pFile );
				if( nReadCount == 0 )
					break;
				::fwrite( buff, 1, nReadCount, pChannelFile );
				::fflush( pChannelFile );
			}
			fclose( pFile );
		}
		
		lpszFile = wcstok( NULL, _T( "|" ) );
	}
	
	strFiles.ReleaseBuffer();
	if( pChannelFile != NULL )
		fclose( pChannelFile );

	return TRUE;
}

void CDlgFileConfig::DeleteMergedFile()
{
	DeleteFile( m_strMergedFile );
}

BOOL CDlgFileConfig::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
        return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFileConfig::ChangeDialogFont(CFont *pFont)
{
	CRect windowRect;

	// grab old and new text metrics
	TEXTMETRIC tmOld, tmNew;
	CDC * pDC = GetDC();
	CFont * pSavedFont = pDC->SelectObject(GetFont());
	pDC->GetTextMetrics(&tmOld);
	pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tmNew);
	pDC->SelectObject(pSavedFont);
	ReleaseDC(pDC);

	CRect rcStcType;
	CRect rcCmbType;

		
	CWnd *pPWnd = GetParent();
	CDC *pPDC = pPWnd->GetDC();
	pPDC->GetTextMetrics(&tmNew);		
	pPWnd->ReleaseDC(pPDC);	

	CDialog *pPPWnd = (CDialog *)pPWnd->GetParent();
	CWnd *pStcWnd = pPPWnd->GetDlgItem(IDC_STC_TYPE);
	CWnd *pCmbWnd = pPPWnd->GetDlgItem(IDC_MSSIM_TYPE);
	pStcWnd->GetWindowRect(&rcStcType);
	pCmbWnd->GetWindowRect(&rcCmbType);

	CRect rc;
	pPWnd->GetWindowRect(&rc);
	CRect newRC;
	newRC.left = newRC.top = 0;
	newRC.right = rc.Width();
	newRC.bottom = rc.Height();
	MoveWindow(&newRC);

	long oldHeight = tmOld.tmHeight+tmOld.tmExternalLeading;
	long newHeight = tmNew.tmHeight+tmNew.tmExternalLeading;

	CRect clientRect;
	GetClientRect(clientRect);
	SetFont(pFont);

	ScreenToClient(rcStcType);
	ScreenToClient(rcCmbType);

	// iterate through and move all child windows and change their font.
	UINT agID[] = {IDC_FC_STC_FILE,IDC_FC_EDT_FILE,IDC_FC_BTN_BRW,IDC_FC_RDO_DIAG,IDC_FC_RDO_LOG};
	int  nCount = sizeof(agID)/sizeof(UINT);

	for(int i=0; i<nCount; i++)
	{
		CWnd *pChildWnd = GetDlgItem(agID[i]);
		pChildWnd->SetFont(pFont);
		pChildWnd->GetWindowRect(windowRect);  

		CString strClass;
		::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
		strClass.MakeUpper();
		if(strClass==_T("COMBOBOX"))
		{
			CRect rect;
			pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
			windowRect.right = rect.right;
			windowRect.bottom = rect.bottom;	

			pChildWnd->SetFocus();
		}		

		ScreenToClient(windowRect);		

		if(agID[i] == IDC_FC_STC_FILE)
		{
			windowRect.left = rcStcType.left;
			windowRect.right = rcStcType.right;
		}
		else if(agID[i] == IDC_FC_EDT_FILE)
		{
			windowRect.left = rcCmbType.left;
			windowRect.right = rcCmbType.right - 50;
		}
		else if(agID[i] == IDC_FC_BTN_BRW)
		{
			windowRect.left = rcCmbType.right-45;
			windowRect.right = rcCmbType.right;
		}
		
		//windowRect.left = XX_MulDiv(windowRect.left,tmNew.tmAveCharWidth,tmOld.tmAveCharWidth);//windowRect.left * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		//windowRect.right = XX_MulDiv(windowRect.right,tmNew.tmAveCharWidth,tmOld.tmAveCharWidth);//windowRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
	
		windowRect.top = XX_MulDiv(windowRect.top,newHeight,oldHeight);//windowRect.top * newHeight / oldHeight;
		windowRect.bottom = XX_MulDiv(windowRect.bottom,newHeight,oldHeight);//windowRect.bottom * newHeight / oldHeight;

		if(windowRect.right > clientRect.right)
		{
			windowRect.right = clientRect.right;
		}

		pChildWnd->MoveWindow(windowRect);


	}

}
int CDlgFileConfig::GetFlowType()
{
	IProtocolPackage *pSmpPack = NULL;
	CreateProtoPackage( &pSmpPack , PT_SMP);

	CString strFile = GetCurFile(0);

	if(strFile.IsEmpty())
		return 0;

	CFileFind Finder;
	if(!Finder.FindFile(strFile))
		return 0;

	HANDLE hFile = CreateFile(strFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if(hFile == INVALID_HANDLE_VALUE)
		return 0;

	LONGLONG llSize =0;
	GetFileSizeEx(hFile,(PLARGE_INTEGER)&llSize);

	DWORD dwSize = (DWORD)(llSize > (128*1024) ? (128*1024):llSize);

	BYTE* lpBuf = new BYTE[dwSize];
	if(lpBuf == NULL)
		return 0;

	DWORD dwRead = 0;
	if(!ReadFile(hFile,lpBuf,dwSize,&dwRead,NULL) || dwRead != dwSize)
	{
		return 0;
	}
	CloseHandle(hFile);

	int nCount = pSmpPack->Append(lpBuf,dwSize);

	pSmpPack->Clear();

	ReleaseProtoPackage(pSmpPack);

	delete [] lpBuf;

	if(nCount > 0)
	{
		return 1; // SMP
	}
	else
	{
		return 0; // DIAG;
	}	
}