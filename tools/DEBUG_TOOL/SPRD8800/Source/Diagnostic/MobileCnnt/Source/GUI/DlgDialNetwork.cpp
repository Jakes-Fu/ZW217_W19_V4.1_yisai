// DlgDialNetwork.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DlgDialNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDialNetwork dialog


CDlgDialNetwork::CDlgDialNetwork(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDialNetwork::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDialNetwork)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(m_szFileName, 0, MAX_PATH);
	m_uDialSecond = 0;
	m_uDialNum = 0;
	m_strContIn = _T("");
	m_uComboxIndexBak = 0;
	m_uUpSpeedBak = 0;
	m_uDownSpeedBak = 0;
	m_uSpeedUp = 0;
	m_uSpeedDown = 0;
	m_strContInBak = _T("");

	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}


void CDlgDialNetwork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDialNetwork)
	DDX_Control(pDX, IDC_COMBO_DIAL_TYPE, m_typeBox);
	DDX_Text(pDX, IDC_EDT_DIAL_SECOND, m_uDialSecond);
	DDX_Text(pDX, IDC_EDT_DIAL_NUM, m_uDialNum);
	DDX_Text(pDX, IDC_EDT_CNNT_IN, m_strContIn);
	DDX_Control(pDX, IDC_COMBO_UP_SPEED, m_boxSpeedUP);
	DDX_Control(pDX, IDC_COMBO_DOWN_SPEED, m_boxSpeedDown);
	DDX_Control(pDX, IDC_BTN_NETWORK_APPLY, m_btnApply);
	DDX_Text(pDX, IDC_STC_SPEED_UP, m_uSpeedUp);
	DDX_Text(pDX, IDC_STC_SPEED_DOWN, m_uSpeedDown);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDialNetwork, CDialog)
	//{{AFX_MSG_MAP(CDlgDialNetwork)
#ifdef SPRD8800
	ON_WM_CTLCOLOR()
#endif
	ON_BN_CLICKED(IDC_BTN_NETWORK_APPLY, OnBtnNetworkApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDialNetwork message handlers

HBRUSH CDlgDialNetwork::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

BOOL CDlgDialNetwork::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strText;
	strText.LoadString(DIAL_TYPE_0);
	m_typeBox.AddString(strText);
	strText.LoadString(DIAL_TYPE_1);
	m_typeBox.AddString(strText);
	strText.LoadString(DIAL_TYPE_2);
	m_typeBox.AddString(strText);
	strText.LoadString(DIAL_TYPE_3);
	m_typeBox.AddString(strText);
	
	m_typeBox.SetCurSel(0);
	
	for(int i = 0; i < NUM_SPEED_UP; i++)
	{
		m_boxSpeedUP.AddString(strSpeedUp[i]);
	}

	m_boxSpeedUP.SetCurSel(0);

	for(i = 0; i < NUM_SPEED_DOWN; i++)
	{
		m_boxSpeedDown.AddString(strSpeedDown[i]);
	}
	
	m_boxSpeedDown.SetCurSel(0);
	
    _tcscpy(m_szFileName,theApp.m_szCfgPathName);

    m_uDialSecond = GetPrivateProfileInt( _T("Net"), _T("DialTime"), 0, m_szFileName );
	m_uDialNum = GetPrivateProfileInt( _T("Net"), _T("DialNum"), 0, m_szFileName );

	UpdateData(FALSE);
	SendCheckRequest();
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDialNetwork::WriteIniFile()
{
	UpdateData();
	
	CString strValue;

	strValue.Format(_T("%d"), m_uDialSecond);
    WritePrivateProfileString(_T("Net"), _T("DialTime"), strValue, m_szFileName);

	strValue.Format(_T("%d"), m_uDialNum);
    WritePrivateProfileString(_T("Net"), _T("DialNum"), strValue, m_szFileName);
}

void CDlgDialNetwork::SendCheckRequest()
{
	UpdateData();
	if(!m_strContIn.IsEmpty() && m_uSpeedUp != 0 && m_uSpeedDown != 0)
	{
		return;
	}
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(SPTEST_READ, 0, NULL, 0);
		
		theApp.m_pEtionFactory->SendATCmd(CGDCONT_READ, 0, NULL, 0);
		
		theApp.m_pEtionFactory->SendATCmd(SRATE_READ, 0, NULL, 0);
	}
}

void CDlgDialNetwork::SendSetRequest()
{
	UpdateData();
	BOOL bFailed = FALSE;

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};
		int nIndex = m_typeBox.GetCurSel();

		int nCurSel = m_boxSpeedUP.GetCurSel();

		CString strText = _T("");
		m_boxSpeedUP.GetLBText(nCurSel, strText);
		
#ifdef _UNICODE
			swscanf(strText, _T("%d"), &m_uSpeedUp);
#else
			sscanf(strText, _T("%d"), &m_uSpeedUp);
#endif	
		nCurSel = m_boxSpeedDown.GetCurSel();
		m_boxSpeedDown.GetLBText(nCurSel, strText);	

#ifdef _UNICODE
			swscanf(strText, _T("%d"), &m_uSpeedDown);
#else
			sscanf(strText, _T("%d"), &m_uSpeedDown);
#endif

		
		TCHAR szPara[MAX_PATH] = {0};
		if(nIndex != (int)m_uComboxIndexBak)
		{
#ifdef _UNICODE
			swprintf(szPara, _T("%d"), nIndex + 1);
#else
			sprintf(szPara, _T("%d"), nIndex + 1);
#endif
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(DIAL_TYPE_SET, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet != NULL)
			{
				strcpy(szRet, lpRet);
			}
			
			if(strstr(szRet, "ERROR") != NULL || szRet[0] == '\0')
			{
				AfxMessageBox(IDS_DIAL_TYPE_FAILED, MB_ICONERROR);
				bFailed = TRUE;
			}
			
		}
		
		
		if(!m_strContIn.IsEmpty() && m_strContIn != m_strContInBak)
		{
			_tcscpy(szPara, m_strContIn);
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(CONT_IN_SET, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet == NULL)
			{
				ZeroMemory(szRet, MAX_PATH);
			}
			else
			{
				strcpy(szRet, lpRet);
			}
			
			if(strstr(szRet, "ERROR") != NULL || szRet[0] == '\0')
			{
				AfxMessageBox(IDS_CONNIN_FAILED, MB_ICONERROR);
				bFailed = TRUE;
			}
		}
		
		if(m_uUpSpeedBak != m_uSpeedUp || m_uDownSpeedBak != m_uSpeedDown)
		{
#ifdef _UNICODE
			swprintf(szPara, _T("%d,%d"), m_uSpeedUp, m_uSpeedDown);
#else
			sprintf(szPara, _T("%d,%d"), m_uSpeedUp, m_uSpeedDown);
#endif
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(SPEED_SET, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet == NULL)
			{
				ZeroMemory(szRet, MAX_PATH);
			}
			else
			{
				strcpy(szRet, lpRet);
			}
			
			if(strstr(szRet, "ERROR") != NULL || szRet[0] == '\0')
			{
				AfxMessageBox(IDS_SPEED_FAILED, MB_ICONERROR);
				bFailed = TRUE;
				m_uSpeedUp = m_uUpSpeedBak;
				m_uSpeedDown = m_uDownSpeedBak;
			}
		}

		if(!bFailed)
		{
			AfxMessageBox(IDS_SET_SUCCESS);
			m_strContInBak = m_strContIn;
			m_uComboxIndexBak = nIndex;
			m_uUpSpeedBak = m_uSpeedUp;
			m_uDownSpeedBak = m_uSpeedDown;
			UpdateData(FALSE);
		}
		
	}

	
}

void CDlgDialNetwork::ShowRespondContent(LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = -1;

	if((nIndex = strText.Find(_T("+CGDCONT:1,")) ) != -1)
	{
		strText.Delete(0, nIndex + 11);
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
				m_strContIn = strText.Left(nIndex);
				m_strContInBak = m_strContIn;
			}
		}

	}
	if((nIndex = strText.Find(_T("+SPPSRATE:2,")) ) != -1)
	{
		strText.Delete(0, nIndex + 12);
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{

#ifdef _UNICODE
			swscanf(strText.Left(nIndex), _T("%d"), &m_uSpeedUp);
#else
			sscanf(strText.Left(nIndex), _T("%d"), &m_uSpeedUp);
#endif
			
			m_uUpSpeedBak = m_uSpeedUp;
			strText.Delete(0, nIndex + 1);

#ifdef _UNICODE
			swscanf(strText, _T("%d"), &m_uSpeedDown);
#else
			sscanf(strText, _T("%d"), &m_uSpeedDown);
#endif
			
			m_uDownSpeedBak = m_uSpeedDown;
		}
		
	}
	if((nIndex = strText.Find(_T("+SPTEST:6,")) ) != -1)
	{
		strText.Delete(0, nIndex + 10);
		if(strText == _T("1"))
		{
			m_typeBox.SetCurSel(0);
			m_uComboxIndexBak = 0;
		}
		else if(strText == _T("2"))
		{
			m_typeBox.SetCurSel(1);
			m_uComboxIndexBak = 1;
		}
		else if(strText == _T("3"))
		{
			m_typeBox.SetCurSel(2);
			m_uComboxIndexBak = 2;
		}
		else if(strText == _T("4"))
		{
			m_typeBox.SetCurSel(3);
			m_uComboxIndexBak = 3;
		}
	}
	UpdateData(FALSE);
}

void CDlgDialNetwork::OnBtnNetworkApply() 
{
	// TODO: Add your control notification handler code here
	WriteIniFile();
	SendSetRequest();
	
}

void CDlgDialNetwork::ClearAll()
{
	memset(m_szFileName, 0, MAX_PATH);
	m_uDialSecond = 0;
	m_uDialNum = 0;
	m_uSpeedDown = 0;
	m_uSpeedUp = 0;
	m_strContIn = _T("");
	m_uComboxIndexBak = 0;
	m_uUpSpeedBak = 0;
	m_uDownSpeedBak = 0;
	m_strContInBak = _T("");

	UpdateData(FALSE);
}