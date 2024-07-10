// DlgUartConfig.cpp : implementation file
//

#include "stdafx.h"
#include "channelserver.h"
#include "DlgUartConfig.h"
#include "MainFrm.h"
#include "ConfigureSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUartConfig dialog

CDlgUartConfig::CDlgUartConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUartConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUartConfig)
	m_bTmpNameMatch = FALSE;
	m_bTmpPortReset = FALSE;
	m_nTmpLogBaudrate = 460800;
	m_nTmpDiagBaudrate = 460800;
	//}}AFX_DATA_INIT
	m_bNameMatch = m_bTmpNameMatch;
	m_bPortReset = m_bTmpPortReset;
	m_nDiagBaudrate = m_nTmpDiagBaudrate;
	m_nLogBaudrate = m_nTmpLogBaudrate;
	m_nLogPort = 0;
	m_nDiagPort = 0;
	m_nEnumPortDelay = 6000;
	m_nEndian = ENDIAN_LITTLE;  
}


void CDlgUartConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUartConfig)
	DDX_Check(pDX, IDC_UC_CHK_NAME_MATCH, m_bTmpNameMatch);
	DDX_Check(pDX, IDC_UC_CHK_PORT_RESET, m_bTmpPortReset);
	DDX_Text(pDX, IDC_UC_EDIT_LOG_BAUD, m_nTmpLogBaudrate);
	DDX_Text(pDX, IDC_UC_EDT_DIAG_BAUD, m_nTmpDiagBaudrate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgUartConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgUartConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUartConfig message handlers

BOOL CDlgUartConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nTmpLogBaudrate = m_nLogBaudrate;
	m_nTmpDiagBaudrate = m_nDiagBaudrate;
	m_bTmpNameMatch = m_bNameMatch;
	m_bTmpPortReset = m_bPortReset;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Modify combobox style
BOOL CDlgUartConfig::RecreatePortCtrl(int nID,  BOOL bList )
{
	CComboBox* pBox = (CComboBox*)GetDlgItem( nID );
	
	CRect rc;
	pBox->GetWindowRect( &rc );
	ScreenToClient( rc );
	rc.bottom = rc.top + 200;
	CWnd* pWnd = pBox->GetParent();
	DWORD dwStyle = pBox->GetStyle();
	if( bList )
	{
		dwStyle &= ~CBS_SIMPLE;	
	}
	else
	{
		dwStyle |= CBS_SIMPLE;
	}

	CFont* pFont = pBox->GetFont();
	
	pBox->DestroyWindow();
	BOOL bRet = pBox->Create( dwStyle,rc,pWnd,nID );
	if( bRet )
	{
		pBox->SetFont( pFont,FALSE );
	}
	
	return bRet;
}

void CDlgUartConfig::GetUartInfo()
{
 	CMainFrame *pMF = (CMainFrame *)AfxGetMainWnd();
	m_vPorts.clear();
	DEV_INFO *pPortInfo=NULL;
	int nCount = pMF->m_pDevHound->ScanPort(&pPortInfo);
	for(int i =0 ; i< nCount; i++)
	{
		DEV_INFO di = pPortInfo[i];
		m_vPorts.push_back(di);	
	}
	pMF->m_pDevHound->FreeMemory(pPortInfo);
	UpdateControls(TRUE);
	UpdateControls(FALSE);

}

void CDlgUartConfig::UpdateControls(BOOL bDiag)
{
	UINT		nID = bDiag?IDC_UC_CMB_DIAG_PORT : IDC_UC_CMB_LOG_PORT;
	CComboBox *pCmb = (CComboBox *)GetDlgItem(nID);
	UINT  nSel      =0xFFFFFFFF;	
	UINT  nPort     = bDiag?m_nDiagPort : m_nLogPort;
//	DWORD dwStyle   = pCmb->GetStyle();
	UINT  nCount    = m_vPorts.size();
	pCmb->ResetContent();	
/*
	if( ( nCount > 0 && !( dwStyle & CBS_SIMPLE ) ) || 
		( nCount == 0 && ( dwStyle & CBS_SIMPLE ) ) )
	{
		RecreatePortCtrl(nID, nCount == 0 );
		pCmb = (CComboBox *)GetDlgItem(nID);
	}	
*/
	pCmb->AddString( _T("None") );
	pCmb->SetItemData( 0,0 );
	
	if( nCount > 0 )
	{
		for( DWORD i=0;i<nCount;i++ )
		{
			pCmb->AddString( m_vPorts[i].szFriendlyName );
			pCmb->SetItemData( i+1,m_vPorts[i].nPortNum );

     		// Compare uart name
			if( 0xFFFFFFFF == nSel && IsRightPort(m_vPorts[i].nPortNum,m_vPorts[i].szFriendlyName,bDiag))
			{
				nSel = i+1;	
			}
		}
		
		if( 0xFFFFFFFF == nSel )
		{
			// Can not find port,use the default
			nSel = 0;
		}
		pCmb->SetCurSel( nSel );
	}
	else
	{  
		if(nPort != 0)
		{
			CString strText;
			strText.Format( _T("%u"),nPort );
			pCmb->SetWindowText( strText );
		}
		else
		{
			pCmb->SetCurSel( 0 );
		}		
	}
}

void CDlgUartConfig::UpdatePorts(BOOL bDiag)
{
	CComboBox *pCmb    = (CComboBox *)GetDlgItem(bDiag?IDC_UC_CMB_DIAG_PORT : IDC_UC_CMB_LOG_PORT);
	UINT      &nPort   = bDiag?m_nDiagPort : m_nLogPort;
	CString   &strName = bDiag?m_strDiagPortName : m_strLogPortName;
	_GetPortInfo(pCmb,nPort,strName);
}

BOOL CDlgUartConfig::IsRightPort(DWORD nPort,LPCTSTR lpszPortName,BOOL bDiag /*= TRUE*/)
{
	UINT &nProPort =bDiag?m_nDiagPort:m_nLogPort;
	CString &strPortName = bDiag?m_strDiagPortName:m_strLogPortName;
	CStringArray &agPorts = bDiag?m_agDiagPortNames:m_agLogPortNames;
	
	if(!m_bNameMatch)
	{
		if(nProPort == nPort)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if(strPortName.IsEmpty())
		{
			return FALSE;
		}
		
		CString strDst = strPortName;
		strDst.MakeLower();
		
		CString strSrc = lpszPortName;
		strSrc.MakeLower();
		if(strSrc.Find(strDst) != -1)
		{
			nProPort = nPort;
			return TRUE;
		}
		
		int nCount = agPorts.GetSize();
		
		for(int i =0; i< nCount && m_bPortReset; i++)
		{
			CString strDst = agPorts[i];
			strDst.MakeLower();
			
			if(strSrc.Find(strDst) != -1)
			{
				nProPort = nPort;
				strPortName = lpszPortName;
				return TRUE;
			}
		}	
		
		return FALSE;
	}
}
void CDlgUartConfig::LoadPortFilters(LPCTSTR lpszConfig)
{
	_TCHAR szBuf[1024]={0};  
	
	GetPrivateProfileString(_T("Uart"), _T("DiagPortFilter"),_T(""),szBuf,1024,lpszConfig);
	SplitStr(szBuf,m_agDiagPortNames);

	memset(szBuf,0,sizeof(szBuf));
	GetPrivateProfileString(_T("Uart"), _T("LogPortFilter"),_T(""),szBuf,1024,lpszConfig);
	SplitStr(szBuf,m_agLogPortNames);
}


void CDlgUartConfig::LoadConfig(LPCTSTR pszCfgFile)
{
    TCHAR szTmp[MAX_PATH] = {0};
	CString strSec = _T("UART");

	LoadPortFilters(pszCfgFile);
	
	m_nDiagPort = GetPrivateProfileInt( strSec, _T("DiagPort"), 0, pszCfgFile );
	m_nDiagBaudrate = GetPrivateProfileInt( strSec, _T("DiagBaudrate"), 460800, pszCfgFile );
	GetPrivateProfileString( strSec, _T("DiagPortName"), _T(""), szTmp, MAX_PATH, pszCfgFile );
	m_strDiagPortName = szTmp;

	memset(szTmp,0,sizeof(szTmp));
	
	m_nLogPort = GetPrivateProfileInt( strSec, _T("LogPort"), 0, pszCfgFile );
	m_nLogBaudrate = GetPrivateProfileInt( strSec, _T("LogBaudrate"), 460800, pszCfgFile );
	GetPrivateProfileString( strSec, _T("LogPortName"), _T(""), szTmp, MAX_PATH, pszCfgFile );
	m_strLogPortName = szTmp;
	
	m_bNameMatch = GetPrivateProfileInt( strSec, _T("NameMatch"), 0, pszCfgFile );
	m_bPortReset = GetPrivateProfileInt( strSec, _T("PortResetWhenAssert"), 0, pszCfgFile );

	m_nEnumPortDelay =  GetPrivateProfileInt( strSec, _T("EnumPortDelayTime"), 6000, pszCfgFile );
}

void CDlgUartConfig::SaveConfig(LPCTSTR pszCfgFile)
{
	CString strTmp;
	CString strSec = _T("UART");

	strTmp.Format(_T("%d"),m_nDiagPort);
	WritePrivateProfileString( strSec, _T("DiagPort"), strTmp, pszCfgFile );
	strTmp.Format(_T("%d"),m_nDiagBaudrate);
	WritePrivateProfileString( strSec, _T("DiagBaudrate"), strTmp, pszCfgFile );
	WritePrivateProfileString( strSec, _T("DiagPortName"), m_strDiagPortName, pszCfgFile );

	strTmp.Format(_T("%d"),m_nLogPort);
	WritePrivateProfileString( strSec, _T("LogPort"), strTmp, pszCfgFile );
	strTmp.Format(_T("%d"),m_nLogBaudrate);
	WritePrivateProfileString( strSec, _T("LogBaudrate"), strTmp, pszCfgFile );
	WritePrivateProfileString( strSec, _T("LogPortName"), m_strLogPortName, pszCfgFile );
		
	strTmp.Format(_T("%d"),m_bNameMatch);
	WritePrivateProfileString( strSec, _T("NameMatch"), strTmp, pszCfgFile );

	strTmp.Format(_T("%d"),m_bPortReset);
	WritePrivateProfileString( strSec, _T("PortResetWhenAssert"), strTmp, pszCfgFile );
}

BOOL CDlgUartConfig::CheckConfig()
{
	UpdateData();
	CComboBox *pCmb    = (CComboBox *)GetDlgItem(IDC_UC_CMB_DIAG_PORT);
	UINT nDiagPort = 0; //pCmb->GetItemData(pCmb->GetCurSel());
	CString strDiagName;
	_GetPortInfo(pCmb,nDiagPort,strDiagName);
	pCmb    = (CComboBox *)GetDlgItem(IDC_UC_CMB_LOG_PORT);
	UINT nLogPort  = 0; //pCmb->GetItemData(pCmb->GetCurSel());
	CString strLogName;
	_GetPortInfo(pCmb,nLogPort,strDiagName);

	if(nDiagPort == nLogPort && nDiagPort != 0)
	{
		AfxMessageBox( _T("Input Error: Diag port can not be same as log port!"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	if(nDiagPort != 0 && m_nTmpDiagBaudrate == 0)
	{
		AfxMessageBox( _T("Input Error: Diag Baudrate can not be zero!"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	if(nLogPort != 0 && m_nTmpLogBaudrate == 0)
	{
		AfxMessageBox( _T("Input Error: Log Baudrate can not be zero!"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	return TRUE;
}
void CDlgUartConfig::UpdateConfig()
{
	UpdateData();
	UpdatePorts(TRUE);
	UpdatePorts(FALSE);	
	m_nDiagBaudrate = m_nTmpDiagBaudrate;
	m_nLogBaudrate  = m_nTmpLogBaudrate;
	m_bNameMatch    = m_bTmpNameMatch;
	m_bPortReset    = m_bTmpPortReset;
}

void CDlgUartConfig::_GetPortInfo(CComboBox *pCmb,	UINT &nPort,CString  &strName)
{
	int       nCount  = (int)m_vPorts.size();	
	//DWORD dwStyle = pCmb->GetStyle();
	//if( dwStyle & CBS_SIMPLE )
	if(nCount>0)
	{
		CString strText;
		pCmb->GetWindowText( strText );		
		int nSel = pCmb->GetCurSel();
		nPort = pCmb->GetItemData( nSel );
		if((nSel-1)>=0 && (nSel-1) < nCount)
		{
			strName = m_vPorts[nSel-1].szDescription;
		}
		else
		{
			strName = _T("None");
		}
	}
	else
	{
		CString strText;
		pCmb->GetWindowText( strText );
		if(strText.CompareNoCase(_T("None"))!=0)
		{
			nPort = _ttoi( strText );
			strName = strText;
		}
		else
		{
			strName = _T("None");
		}
	}
}
BOOL CDlgUartConfig::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
        return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgUartConfig::ChangeDialogFont(CFont *pFont)
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
	UINT agID[] = {
		IDC_UC_STC_DIAG_PORT, IDC_UC_CMB_DIAG_PORT,
		IDC_UC_STC_DIAG_BAUD, IDC_UC_EDT_DIAG_BAUD,
	    IDC_UC_STC_LOG_PORT,  IDC_UC_CMB_LOG_PORT,
	    IDC_UC_STC_LOG_BAUD,  IDC_UC_EDIT_LOG_BAUD,
	    IDC_UC_CHK_PORT_RESET,IDC_UC_CHK_NAME_MATCH};

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

		if(agID[i] == IDC_UC_STC_DIAG_PORT || 
			agID[i] == IDC_UC_STC_DIAG_BAUD || 
			agID[i] == IDC_UC_STC_LOG_PORT || 
			agID[i] == IDC_UC_STC_LOG_BAUD )
		{
			windowRect.left = rcStcType.left;
			windowRect.right = rcStcType.right;
		}
		else if(agID[i] == IDC_UC_CMB_DIAG_PORT || 
			agID[i] == IDC_UC_EDT_DIAG_BAUD || 
			agID[i] == IDC_UC_CMB_LOG_PORT || 
			agID[i] == IDC_UC_EDIT_LOG_BAUD )
		{
			windowRect.left = rcCmbType.left;
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