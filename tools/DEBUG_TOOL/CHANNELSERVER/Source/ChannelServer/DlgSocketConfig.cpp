// DlgSocketConfig.cpp : implementation file
//

#include "stdafx.h"
#include "channelserver.h"
#include "DlgSocketConfig.h"
#include <Winsock2.h>
#include <ATLCONV.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSocketConfig dialog


CDlgSocketConfig::CDlgSocketConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSocketConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSocketConfig)
	m_nTmpPort = 36667;
	m_nTmpFlowType = 0;
	//}}AFX_DATA_INIT
	m_strIP = _T("127.0.0.1");
	m_nPort = m_nTmpPort;
	m_nIP   = 0x7F000001;
	m_nEndian = ENDIAN_LITTLE;
}


void CDlgSocketConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSocketConfig)
	DDX_Control(pDX, IDC_SC_IPADDRESS, m_ctrIP);
	DDX_Text(pDX, IDC_SC_EDT_PORT, m_nTmpPort);
	DDX_Radio(pDX, IDC_SC_RDO_DIAG, m_nTmpFlowType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSocketConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgSocketConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSocketConfig message handlers
BOOL CDlgSocketConfig::CheckPort()
{
	UpdateData();

	if( m_nTmpPort >= 1024 && m_nTmpPort <= 65536  )
		return TRUE;
	
    AfxMessageBox( _T("Input Error: Socket Port is invalid!"), MB_OK | MB_ICONERROR );
	
    m_nTmpPort = m_nPort;
	UpdateData(FALSE);

    return FALSE;
}
BOOL CDlgSocketConfig::CheckIP()
{
	DWORD dwIP;
	if(m_ctrIP.GetAddress(dwIP) == 4)
		return TRUE;
	
    AfxMessageBox( _T("Input Error: Socket IP is invalid!"), MB_OK | MB_ICONERROR );
    m_ctrIP.SetWindowText( m_strIP );
    return FALSE;
}

BOOL CDlgSocketConfig::CheckConfig()
{
	if(!CheckPort())
		return FALSE;
	if(!CheckIP())
		return FALSE;
	return TRUE;
}

BOOL CDlgSocketConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nTmpPort = m_nPort;
	m_nTmpFlowType = m_nFlowType;
	m_ctrIP.SetWindowText(m_strIP);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSocketConfig::LoadConfig(LPCTSTR pszCfgFile)
{
	USES_CONVERSION;
	_TCHAR szTmp[MAX_PATH] = {0};

	CString strSec = _T("Socket");

	m_nPort = GetPrivateProfileInt( strSec, _T("Port"), 36667, pszCfgFile );

	GetPrivateProfileString( strSec, _T("IP"), _T("127.0.0.1"), szTmp, MAX_PATH, pszCfgFile );
	m_strIP = szTmp;
	m_nIP = inet_addr(T2A(szTmp));
	m_nIP = htonl(m_nIP);
	m_nFlowType = GetPrivateProfileInt( strSec, _T("FlowType "), 0, pszCfgFile );  
}

void CDlgSocketConfig::SaveConfig(LPCTSTR pszCfgFile)
{
	CString strTmp;	
	CString strSec = _T("Socket");

	strTmp.Format(_T("%d"),m_nPort);
	WritePrivateProfileString( strSec,_T("Port"),strTmp,pszCfgFile);
	
	WritePrivateProfileString( strSec,_T("IP"), m_strIP, pszCfgFile );

	strTmp.Format(_T("%d"),m_nTmpFlowType);
	WritePrivateProfileString( strSec,_T("FlowType"),strTmp,pszCfgFile);

}
void CDlgSocketConfig::UpdateConfig()
{
	UpdateData();
	m_nPort = m_nTmpPort;
	m_ctrIP.GetAddress(m_nIP);
	m_ctrIP.GetWindowText(m_strIP);
}
BOOL CDlgSocketConfig::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
        return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSocketConfig::ChangeDialogFont(CFont *pFont)
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
	UINT agID[] = {IDC_SC_STC_PORT,IDC_SC_EDT_PORT,IDC_SC_STC_IP,IDC_SC_IPADDRESS,IDC_SC_RDO_DIAG,IDC_SC_RDO_LOG};
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

		if(agID[i] == IDC_SC_STC_PORT || agID[i] == IDC_SC_STC_IP )
		{
			windowRect.left = rcStcType.left;
			windowRect.right = rcStcType.right;
		}
		else if(agID[i] == IDC_SC_EDT_PORT || agID[i] == IDC_SC_IPADDRESS)
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