// DlgNtac.cpp : implementation file
//

#include "stdafx.h"
#include "ResOver.h"
#include "DlgNtac.h"
#include "NAParserV2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNtac dialog


CDlgNtac::CDlgNtac(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNtac::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNtac)
	m_nAccessType = -1;
	m_nAccountType = -1;
	m_nIPType = -1;
	m_nMvnoType = -1;
	m_nRoamingPro = -1;
	m_strApn = _T("");
	m_strHomePage = _T("");
	m_nUiorder = 0;
	m_nMcc = 0;
	m_nMnc = 0;
	m_strPassword = _T("");
	m_nPort = 0;
	m_strName = _T("");
	m_strUserName = _T("");
	m_nAccessOption = -1;
	m_nAuthType = -1;
	m_bActive = FALSE;
	m_bVisible = FALSE;
	m_bRoaming = FALSE;
	m_strPos = _T("");
	m_strUsed = _T("");
	m_nIndex = 1;
	m_strGateWay = _T("");
	//}}AFX_DATA_INIT

	m_nCurIndex = -1;
	m_strNTAC = _T("");
	m_strTmpPath = _T("");
	m_bViewOnly = FALSE;
}


void CDlgNtac::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNtac)
	DDX_Control(pDX, IDC_IP_DNS2, m_ipDns2);
	DDX_Control(pDX, IDC_IP_DNS1, m_ipDns1);
	DDX_CBIndex(pDX, IDC_CMB_ACCESS_TYPE, m_nAccessType);
	DDX_CBIndex(pDX, IDC_IP_TYPE, m_nIPType);
	DDX_CBIndex(pDX, IDC_COMBO_MVNO_TYPE, m_nMvnoType);
	DDX_CBIndex(pDX, IDC_COMBO_ROAMING_PROTOCOL, m_nRoamingPro);
	DDX_CBIndex(pDX, IDC_CMB_ACCOUNT_TYPE, m_nAccountType);
	DDX_Text(pDX, IDC_EDT_APN, m_strApn);
	DDV_MaxChars(pDX, m_strApn, NTAC_APN_LEN);
	DDX_Text(pDX, IDC_EDT_HOMEPAGE, m_strHomePage);
	DDV_MaxChars(pDX, m_strHomePage, NTAC_HOMEPAGE_LEN);
	DDX_Text(pDX, IDC_EDT_UIORDER, m_nUiorder);
	DDV_MinMaxUInt(pDX, m_nUiorder, 0, 65535);
	DDX_Text(pDX, IDC_EDT_MCC, m_nMcc);
	DDV_MinMaxUInt(pDX, m_nMcc, 0, 65535);
	DDX_Text(pDX, IDC_EDT_MNC, m_nMnc);
	DDV_MinMaxUInt(pDX, m_nMnc, 0, 65535);
	DDX_Text(pDX, IDC_EDT_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, NTAC_PASSWORD_LEN);
	DDX_Text(pDX, IDC_EDT_PORT, m_nPort);
	DDV_MinMaxUInt(pDX, m_nPort, 0, 65535);
	DDX_Text(pDX, IDC_EDT_SETTING_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, NTAC_NAME_LEN);
	DDX_Text(pDX, IDC_EDT_USERNAME, m_strUserName);
	DDV_MaxChars(pDX, m_strUserName, NTAC_USERNAME_LEN);
	DDX_CBIndex(pDX, IDC_CMB_ACCESS_OPTION, m_nAccessOption);
	DDX_Radio(pDX, IDC_RDO_PAP, m_nAuthType_PAP);
	DDX_Radio(pDX, IDC_RDO_CHAP, m_nAuthType_CHAP);
	DDX_Radio(pDX, IDC_RDO_PAPCHAP, m_nAuthType_PAPCHAP);
	DDX_Radio(pDX, IDC_RDO_NONE, m_nAuthType_NONE);
	DDX_Check(pDX, IDC_CHK_ACTIVE, m_bActive);
	DDX_Check(pDX, IDC_CHECK_VISIBLE, m_bVisible);
	DDX_Check(pDX, IDC_CHECK_ROAMING, m_bRoaming);
	DDX_Text(pDX, IDC_STC_POS, m_strPos);
	DDX_Text(pDX, IDC_STC_USED, m_strUsed);
	DDX_Text(pDX, IDC_EDIT_PAGENUM, m_nIndex);
	DDX_Text(pDX, IDC_IP_GATEWAY, m_strGateWay);
	DDV_MaxChars(pDX, m_strGateWay, 128);

	DDX_Text(pDX, IDC_EDT_SPN, m_strSpn);
	DDV_MaxChars(pDX, m_strSpn, NTAC_SPN_LEN);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgNtac, CDialog)
	//{{AFX_MSG_MAP(CDlgNtac)
	ON_BN_CLICKED(IDC_BTN_PREV, OnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, OnNext)
	ON_BN_CLICKED(IDC_CHK_ACTIVE, OnChkActive)
	ON_CBN_SELCHANGE(IDC_CMB_ACCESS_TYPE, OnSelchangeAccessType)
	ON_BN_CLICKED(IDC_BTN_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_BTN_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_BTN_GO, OnGo2Pos)
	ON_BN_CLICKED(IDC_BTN_IST_NTAC, OnPreIstNtac)
	//}}AFX_MSG_MAP
	ON_CONTROL(EN_CHANGE,IDC_EDIT_PAGENUM,OnParsePos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNtac message handlers

BOOL CDlgNtac::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_Parser.m_tHead.nCount > 0)
	{
		m_nCurIndex = 0;
	}
	else
	{
		m_nCurIndex = -1;		
	}
	ShowCurNtac();
	UpdateCtrlState();
	GetDlgItem(IDC_BTN_PREV)->EnableWindow(FALSE);
	UpdateData(FALSE);

	if(m_bViewOnly)
	{
		//this->ModifyStyle(WS_POPUP,WS_CHILD,SWP_DRAWFRAME);
		//CHILD:0x40000044 , 0x00000000
		//FRAME:0x84c800c4 , 0x00010101
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CDlgNtac::SetBuf(LPBYTE pBuf,DWORD dwSize,LPCTSTR lpszTmpPath)
{	
	BOOL bOK = m_Parser.DecodeNTAC(pBuf,dwSize);
	m_strTmpPath = lpszTmpPath;

	if(m_Parser.m_tHead.nCount > 0)
	{
		m_nCurIndex = 0;
	}
	else
	{
		m_nCurIndex = -1;
	}

	if(m_bViewOnly)
	{
		ShowCurNtac();
		UpdateCtrlState();
		GetDlgItem(IDC_BTN_PREV)->EnableWindow(FALSE);		
		if(m_Parser.m_tHead.nCount > 1)
		{
			GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
		}
		UpdateData(FALSE);
	}
	return bOK;
}

void CDlgNtac::ShowCurNtac()
{
	if(m_nCurIndex == -1)
	{
/*		m_strPos.Format(_T("%d/%d"),m_bViewOnly?(m_Parser.m_tHead.nCount==0 ? 0 : (m_nCurIndex+1)) : m_nCurIndex+1 ,
		                        m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vNTAC.size());
								*/
		m_nIndex = m_bViewOnly?(m_Parser.m_tHead.nCount==0 ? 1 : (m_nCurIndex+1)) : m_nCurIndex+1 ;
		m_strPos.Format(_T("/%d"),m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vNTAC.size());

		m_strUsed.Format(_T("USED:%d"),m_Parser.m_tHead.nCount);
		UpdateData(FALSE);
		UpdateCtrlState();
		GetDlgItem(IDC_CHK_ACTIVE)->EnableWindow(FALSE);
		return;
	}
	else
	{
		GetDlgItem(IDC_CHK_ACTIVE)->EnableWindow(TRUE);
	}
	m_nUiorder = m_Parser.m_vNTAC[m_nCurIndex].uiorder;
	m_nMcc = m_Parser.m_vNTAC[m_nCurIndex].mcc;
	m_nMnc = m_Parser.m_vNTAC[m_nCurIndex].mnc;
	m_nPort = m_Parser.m_vNTAC[m_nCurIndex].port;
	m_nAccessType = m_Parser.m_vNTAC[m_nCurIndex].access_type;
	m_nIPType = m_Parser.m_vNTAC[m_nCurIndex].reserved;
	m_nMvnoType = m_Parser.m_vNTAC[m_nCurIndex].mvno_type;
	m_nRoamingPro = m_Parser.m_vNTAC[m_nCurIndex].roaming_protocol;
	m_nAccessOption = m_Parser.m_vNTAC[m_nCurIndex].access_option;
	m_nAccountType = m_Parser.m_vNTAC[m_nCurIndex].account_type;
	m_nAuthType = m_Parser.m_vNTAC[m_nCurIndex].auth_type;

	switch(m_nAuthType)
	{
		case MN_PAP:		m_nAuthType_PAP = 0;m_nAuthType_CHAP = 1;m_nAuthType_PAPCHAP = 1; m_nAuthType_NONE = 1;break;
		case MN_CHAP:		m_nAuthType_PAP = 1;m_nAuthType_CHAP = 0;m_nAuthType_PAPCHAP = 1; m_nAuthType_NONE = 1;break;
		case MN_PAP_CHAP:	m_nAuthType_PAP = 1;m_nAuthType_CHAP = 1;m_nAuthType_PAPCHAP = 0; m_nAuthType_NONE = 1;break;
		case MN_AUTH_NONE:	m_nAuthType_PAP = 1;m_nAuthType_CHAP = 1;m_nAuthType_PAPCHAP = 1; m_nAuthType_NONE = 0;break;
		default:			m_nAuthType_PAP = 1;m_nAuthType_CHAP = 1;m_nAuthType_PAPCHAP = 1; m_nAuthType_NONE = 1;break;
	}

	m_strName = m_Parser.m_vNTAC[m_nCurIndex].name;
	m_strApn = m_Parser.m_vNTAC[m_nCurIndex].apn;
	m_strUserName = m_Parser.m_vNTAC[m_nCurIndex].username;
	m_strPassword = m_Parser.m_vNTAC[m_nCurIndex].password;
	m_strHomePage = m_Parser.m_vNTAC[m_nCurIndex].homepage;
	m_strGateWay = m_Parser.m_vNTAC[m_nCurIndex].gateway;
	m_strSpn = m_Parser.m_vNTAC[m_nCurIndex].mvno_value;

	m_ipDns1.SetWindowText(m_Parser.m_vNTAC[m_nCurIndex].dns1);
	m_ipDns2.SetWindowText(m_Parser.m_vNTAC[m_nCurIndex].dns2);

	m_bActive = m_Parser.m_vNTAC[m_nCurIndex].active_flag;
	m_bVisible = m_Parser.m_vNTAC[m_nCurIndex].Visible;
	m_bRoaming = m_Parser.m_vNTAC[m_nCurIndex].Roaming;

/*	m_strPos.Format(_T("%d/%d"),m_bViewOnly?(m_Parser.m_tHead.nCount==0 ? 0 : (m_nCurIndex+1)) : m_nCurIndex+1 ,
		                        m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vNTAC.size());
								*/
	m_nIndex = m_bViewOnly?(m_Parser.m_tHead.nCount==0 ? 1 : (m_nCurIndex+1)) : m_nCurIndex+1;

	m_strPos.Format(_T("/%d"),m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vNTAC.size());

	m_strUsed.Format(_T("USED:%d"),m_Parser.m_tHead.nCount);

	UpdateData(FALSE);	

	UpdateCtrlState();
}

void CDlgNtac::SaveCurNtac()
{
	UpdateData();

	if(m_nCurIndex == -1)
	{
		return;
	}

	m_Parser.m_vNTAC[m_nCurIndex].uiorder = (WORD)m_nUiorder;
	m_Parser.m_vNTAC[m_nCurIndex].mcc = (WORD)m_nMcc;
	m_Parser.m_vNTAC[m_nCurIndex].mnc = (WORD)m_nMnc;
	m_Parser.m_vNTAC[m_nCurIndex].port = (WORD)m_nPort;
	m_Parser.m_vNTAC[m_nCurIndex].access_type = (BYTE)m_nAccessType ;
	m_Parser.m_vNTAC[m_nCurIndex].reserved = (BYTE)m_nIPType;
	m_Parser.m_vNTAC[m_nCurIndex].mvno_type = (BYTE)m_nMvnoType	;
	m_Parser.m_vNTAC[m_nCurIndex].roaming_protocol = (BYTE)m_nRoamingPro;
	m_Parser.m_vNTAC[m_nCurIndex].access_option = (BYTE)m_nAccessOption;
	m_Parser.m_vNTAC[m_nCurIndex].account_type = (BYTE)m_nAccountType ;
	m_Parser.m_vNTAC[m_nCurIndex].auth_type = (BYTE)m_nAuthType;

	_tcscpy(m_Parser.m_vNTAC[m_nCurIndex].name,m_strName);
	_tcscpy(m_Parser.m_vNTAC[m_nCurIndex].apn,m_strApn);
	_tcscpy(m_Parser.m_vNTAC[m_nCurIndex].username,m_strUserName);
	_tcscpy(m_Parser.m_vNTAC[m_nCurIndex].password,m_strPassword);
	_tcscpy(m_Parser.m_vNTAC[m_nCurIndex].homepage,m_strHomePage);
	_tcscpy(m_Parser.m_vNTAC[m_nCurIndex].gateway,m_strGateWay);
	_tcscpy(m_Parser.m_vNTAC[m_nCurIndex].mvno_value,m_strSpn);
	
	m_ipDns1.GetWindowText(m_Parser.m_vNTAC[m_nCurIndex].dns1,NTAC_IP_LEN+1);
	m_ipDns2.GetWindowText(m_Parser.m_vNTAC[m_nCurIndex].dns2,NTAC_IP_LEN+1);
	m_Parser.m_vNTAC[m_nCurIndex].active_flag = m_bActive;
	m_Parser.m_vNTAC[m_nCurIndex].Visible = (BYTE)m_bVisible;
	m_Parser.m_vNTAC[m_nCurIndex].Roaming = (BYTE)m_bRoaming;
}

void CDlgNtac::OnGo2Pos()
{
	if(m_nCurIndex == -1)
	{
		return;
	}
	if(!UpdateData())
	{
		return;
	}

	int nUpbound = m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vNTAC.size();
	if ( (m_nIndex > nUpbound) ||(m_nIndex <= 0) )
	{
		return;
	}
	if(m_bActive && m_strName.IsEmpty())
	{
		AfxMessageBox(_T("Setting name can not be empty if it is acitve!"));
		GetDlgItem(IDC_EDT_SETTING_NAME)->SetFocus();
		return;
	}
	SaveCurNtac();
	m_nCurIndex = m_nIndex-1;
	if(m_nCurIndex<=0)
	{
		m_nCurIndex = 0;		
	}	
	ShowCurNtac();

	if(m_nCurIndex > 0)
	{
		GetDlgItem(IDC_BTN_PREV)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_PREV)->EnableWindow(FALSE);
	}
}
void CDlgNtac::OnPrev() 
{
	// TODO: Add your control notification handler code here
	if(m_nCurIndex == -1)
	{
		return;
	}
	if(!UpdateData())
	{
		return;
	}
	if(m_bActive && m_strName.IsEmpty())
	{
		AfxMessageBox(_T("Setting name can not be empty if it is acitve!"));
		GetDlgItem(IDC_EDT_SETTING_NAME)->SetFocus();
		return;
	}

	SaveCurNtac();
	m_nCurIndex --;
	if(m_nCurIndex<=0)
	{
		m_nCurIndex = 0;		
	}	
	ShowCurNtac();
	if(m_nCurIndex == 0)
	{
		GetDlgItem(IDC_BTN_PREV)->EnableWindow(FALSE);
	}

	int nUpbound = m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vNTAC.size();

	if(m_nCurIndex < (nUpbound-1))
	{
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
	}
}

void CDlgNtac::OnNext() 
{
	// TODO: Add your control notification handler code here
	if(m_nCurIndex == -1)
	{
		if(!m_bViewOnly)
		{
			if(AfxMessageBox(_T("Do you want to insert one at the tail?"),MB_YESNO) == IDYES)
			{
				NTAC_T ntac;
				ntac.Init();
				m_Parser.m_vNTAC.push_back(ntac);
				m_nCurIndex = 0;
				ShowCurNtac();	
			}
			return;
		}
	}

	if(!UpdateData())
	{
		return;
	}
	if(m_bActive && m_strName.IsEmpty())
	{
		AfxMessageBox(_T("Setting name can not be empty if it is acitve!"));
		GetDlgItem(IDC_EDT_SETTING_NAME)->SetFocus();
		return;
	}
	SaveCurNtac();
	m_nCurIndex++;

	int nUpbound = m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vNTAC.size();

	if(m_nCurIndex >= nUpbound && !m_bViewOnly)
	{
		if(nUpbound < NTAC_MAX_NUM)
		{
			if(AfxMessageBox(_T("Do you want to insert one at the tail?"),MB_YESNO) == IDYES)
			{
				NTAC_T ntac;
				ntac.Init();
				m_Parser.m_vNTAC.push_back(ntac);
				nUpbound ++;
				if(nUpbound == NTAC_MAX_NUM)
				{
					GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
				}
			}
		}
	}

	if(m_nCurIndex >= nUpbound)
	{
		m_nCurIndex = nUpbound-1;	
		if(m_nCurIndex == -1)
		{
			m_nCurIndex = 0;
		}
	}
	ShowCurNtac();
	if(m_nCurIndex == (nUpbound-1) && m_bViewOnly)
	{
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	}

	if(m_nCurIndex > 0)
	{
		GetDlgItem(IDC_BTN_PREV)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_PREV)->EnableWindow(FALSE);
	}
}

void CDlgNtac::OnChkActive() 
{
	// TODO: Add your control notification handler code here
	m_bActive = ((CButton*)GetDlgItem(IDC_CHK_ACTIVE))->GetCheck();

	
	if(m_Parser.m_vNTAC[m_nCurIndex].active_flag && !m_bActive)
	{
		m_Parser.m_tHead.nCount --;
	}
	else if(!m_Parser.m_vNTAC[m_nCurIndex].active_flag && m_bActive)
	{
		m_Parser.m_tHead.nCount ++;
	}

	m_strUsed.Format(_T("USED:%d"),m_Parser.m_tHead.nCount);
	UpdateData(FALSE);
	SaveCurNtac();
	UpdateCtrlState();
}
void CDlgNtac::UpdateCtrlState()
{
	GetDlgItem(IDC_EDT_SETTING_NAME)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_MCC)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_MNC)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_CMB_ACCESS_OPTION)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_CMB_ACCESS_TYPE)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_CMB_ACCOUNT_TYPE)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_IP_DNS1)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_IP_DNS2)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_IP_TYPE)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_COMBO_MVNO_TYPE)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_COMBO_ROAMING_PROTOCOL)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_IP_GATEWAY)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_HOMEPAGE)->EnableWindow(m_bActive && !m_bViewOnly);	
	GetDlgItem(IDC_EDT_PORT)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_RDO_PAP)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_RDO_CHAP)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_RDO_PAPCHAP)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_RDO_NONE)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_USERNAME)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_PASSWORD)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_APN)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_SPN)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_UIORDER)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_CHECK_VISIBLE)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_CHECK_ROAMING)->EnableWindow(m_bActive && !m_bViewOnly);
	if(m_bViewOnly)
	{
		GetDlgItem(IDC_BTN_IMPORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_EXPORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHK_ACTIVE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_USED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_IST_NTAC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_GO)->ShowWindow(SW_HIDE);
	}
}

void CDlgNtac::OnOK() 
{
	// TODO: Add extra validation here
	if(!UpdateData())
	{
		return;
	}
	if(m_bActive && m_strName.IsEmpty())
	{
		AfxMessageBox(_T("Setting name can not be empty if it is acitve!"));
		GetDlgItem(IDC_EDT_SETTING_NAME)->SetFocus();
		return;
	}

	SaveCurNtac();

	LPBYTE pBuf = NULL;
	DWORD  dwSize = 0;
	m_Parser.EncodeNTAC(pBuf,dwSize);

	m_strNTAC.Empty();
	CString strFile;
	strFile.Format(_T("%s\\_ntac.dat"),m_strTmpPath);
	FILE *pFile = _tfopen(strFile.operator LPCTSTR(),_T("wb"));
	if(pFile == NULL)
	{
		AfxMessageBox(_T("Save data failed!"));
		return;
	}
	
	DWORD dwWirte = fwrite(pBuf,1,dwSize,pFile);
	fclose(pFile);
	pFile = NULL;

	if(dwWirte != dwSize)
	{
		AfxMessageBox(_T("Save data failed!"));
		return;
	}

	m_strNTAC = strFile;

	CDialog::OnOK();
}

void CDlgNtac::OnSelchangeAccessType() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_nAccessType == 0)
	{
		m_nPort = 9201;
	}
	else
	{
		m_nPort = 80;
	}
	UpdateData(FALSE);
}

void CDlgNtac::OnImport() 
{
	// TODO: Add your control notification handler code here
	static _TCHAR szFilter[512] = _T("Excel 97-2003 Files (*.xls)|*.xls|")
		_T("Excelx Files (*.xlsx)|*.xlsx|")
		_T("Ntac file(*.*)|*.*|");
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,szFilter);
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}
	BOOL bXlsFile = FALSE;
	CString strOpenFile = dlg.GetPathName();
	if( (_tcsicmp(strOpenFile.Right(4),_T(".xls")) == 0) ||
		(_tcsicmp(strOpenFile.Right(4),_T("xlsx")) == 0))
	{
		bXlsFile = TRUE;
	}
	if (bXlsFile) //Import to excel file
	{
		UINT nRet = 0;
		nRet = m_Parser.ParseNtacSheet(strOpenFile);
		switch(nRet)
		{
		case E_OK:
			break;
		case E_DEC_XLS_ERR:
			AfxMessageBox(_T("Parsed NtacSheet excel file failed!"));
			break;
		default:
			AfxMessageBox(_T("Invalid NtacSheet excel file!"));
			break;
		}
	}
	else
	{
		FILE *pFile = _tfopen(dlg.GetPathName(),_T("rb"));
		if(pFile == NULL)
		{
			CString strErr;
			strErr.Format(_T("Open file \"%s\" failed!"),dlg.GetPathName());
			AfxMessageBox(strErr);
			return;
		}
		fseek(pFile,0,SEEK_END);
		DWORD dwSize = ftell(pFile);
		fseek(pFile,0,SEEK_SET);
		if(dwSize == 0)
		{
			CString strErr;
			strErr.Format(_T("File size is zero!"));
			AfxMessageBox(strErr);
			return;
		}
		LPBYTE pBuf = new BYTE[dwSize];
		DWORD dwRead = fread(pBuf,1,dwSize,pFile);
		
		fclose(pFile);
		
		if(dwRead != dwSize)
		{
			CString strErr;
			strErr.Format(_T("Read file failed!"));
			AfxMessageBox(strErr);
			return;
		}
		
		BOOL bOK = m_Parser.DecodeNTAC(pBuf,dwSize);
		
		if(!bOK)
		{
			CString strErr;
			strErr.Format(_T("Import failed!\nPlease check the file format!"));
			AfxMessageBox(strErr);
		}
		
		delete [] pBuf;		
	}
	if(m_Parser.m_vNTAC.size() > 0)
	{
		m_nCurIndex = 0;
	}
	else
	{
		m_nCurIndex = -1;
	}
	ShowCurNtac();
	UpdateCtrlState();
	GetDlgItem(IDC_BTN_PREV)->EnableWindow(FALSE);
	UpdateData(FALSE);


}

void CDlgNtac::OnExport() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_bActive && m_strName.IsEmpty())
	{
		AfxMessageBox(_T("Setting name can not be empty if it is acitve!"));
		GetDlgItem(IDC_EDT_SETTING_NAME)->SetFocus();
		return;
	}
	
	SaveCurNtac();
	
	if(m_Parser.m_vNTAC.size() == 0)
	{
		AfxMessageBox(_T("No item to save!"));
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	static _TCHAR szFilter[512] = _T("Excel 97-2003 Files (*.xls)|*.xls|")
		_T("Excelx Files (*.xlsx)|*.xlsx|")
		_T("Ntac file(*.ntac)|*.ntac|");
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}
	BOOL bXlsFile = FALSE;
	CString strSaveFile = dlg.GetPathName();
	strSaveFile = strSaveFile.Left(strSaveFile.ReverseFind(_T('\\'))+1);
	strSaveFile += dlg.GetFileTitle();
	
	switch(dlg.m_ofn.nFilterIndex)
	{
	case 1:
		strSaveFile += _T(".xls");                  
		break; 
	case 2:
		strSaveFile += _T(".xlsx");                  
		break; 
	default:
		strSaveFile += _T(".ntac");  
		break;
	}
	
	if(	(_tcsicmp(strSaveFile.Right(4),_T(".xls")) == 0) ||
		(_tcsicmp(strSaveFile.Right(4),_T("xlsx")) == 0) )
	{
		bXlsFile = TRUE;
	}
	if (bXlsFile) //Export to excel file
	{
		if( m_Parser.CreateNtacSheet(strSaveFile))
			AfxMessageBox(_T("Export complete!"));
		else
			AfxMessageBox(_T("Export failed !"));
	}
	else
	{
		BOOL bOK = FALSE;
		FILE *pFile = NULL;
		LPBYTE pBuf = NULL;
		DWORD  dwSize = 0;
		do 
		{
			if (!m_Parser.EncodeNTAC(pBuf,dwSize))
			{
				AfxMessageBox(_T("EncodeNTAC failed!"));
				break;
			}
			pFile = _tfopen(strSaveFile,_T("wb"));
			if(pFile == NULL)
			{
				AfxMessageBox(_T("Create file failed!"));
				break;
			}
			DWORD dwWirte = fwrite(pBuf,1,dwSize,pFile);
			if(dwWirte != dwSize)
			{
				AfxMessageBox(_T("Save data failed!"));
				break;
			}
			bOK = TRUE;
		} while (0);
		if (pFile)
		{
			fclose(pFile);
			pFile = NULL;
		}

		if (pBuf)
		{
			delete[] pBuf;
			pBuf = NULL;
		}
		if(bOK)
		{
			AfxMessageBox(_T("Export complete!"));
		}
		
	}

}
/*
BOOL CDlgNtac::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	if(m_bViewOnly)
	{
		HINSTANCE hInst = AfxFindResourceHandle( MAKEINTRESOURCE( nIDTemplate ), RT_DIALOG);
		HRSRC hResource = ::FindResource(hInst,MAKEINTRESOURCE( nIDTemplate ), RT_DIALOG);
		HGLOBAL hTemplate = LoadResource(hInst, hResource);
		ASSERT( hTemplate );
		LPCDLGTEMPLATE lpDialogTemplate = ( LPCDLGTEMPLATE )LockResource( hTemplate );
		ASSERT( lpDialogTemplate );
		DWORD *ptrStyle = ( DWORD* )&lpDialogTemplate->style;
		*ptrStyle &= ~( WS_OVERLAPPEDWINDOW|WS_POPUPWINDOW );
		*ptrStyle |= WS_CHILDWINDOW;
		UnlockResource( hTemplate );
		BOOL bResult = CreateIndirect(hTemplate, pParentWnd, hInst);
		FreeResource(hTemplate);
		
		return bResult;
	}
	else
	{
		return CDialog::Create(nIDTemplate,pParentWnd);
	}
	
}
*/

BOOL CDlgNtac::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg-> message == WM_KEYDOWN ) 
	{       			
		if(pMsg->hwnd == GetDlgItem(IDC_EDIT_PAGENUM)->m_hWnd) 
		{ 
			if(pMsg->wParam == VK_RETURN) 
			{ 
				OnGo2Pos();
				return TRUE;
			} 
		} 
	}
	if( pMsg->message == WM_CHAR )
	{
		CWnd * pWnd = GetFocus();
		_ASSERTE( pWnd != NULL );
	
		_TCHAR cValue = (_TCHAR)(pMsg->wParam);
		if( pWnd->m_hWnd == GetDlgItem(IDC_EDT_APN)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_EDT_USERNAME)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_EDT_PASSWORD)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_EDT_HOMEPAGE)->m_hWnd) 

		{
			//Ö»ÄÜÊÇASCIIÂë	
			if( cValue >= 0x80 || cValue < 0x20)
			{   
				if(cValue != VK_BACK && cValue != 3 && cValue != 22 && cValue != 24) //Ctrl+c Ctrl+v Ctrl+x
				{
					return TRUE;
				}
			}
		
		}
	
	}

	return CDialog::PreTranslateMessage(pMsg);
}
void CDlgNtac::OnParsePos() 
{
	CString strPosInfo = _T("");
	GetDlgItem(IDC_EDIT_PAGENUM)->GetWindowText(strPosInfo);
	if (strPosInfo.IsEmpty()) //bypass "Backspace" key 
	{
		return;
	}
	UpdateData();
	int nMaxData= m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vNTAC.size();
	if ( (m_nIndex > nMaxData) ||(m_nIndex <= 0) )
	{
		CString str;
		str.Format(_T("Please enter an interger between 1 and %d."),nMaxData);
		AfxMessageBox(str);
		m_nIndex = nMaxData ;
		UpdateData(FALSE);
		::SendMessage(GetDlgItem(IDC_EDIT_PAGENUM)->m_hWnd,EM_SETSEL,0,-1);
	}
}

void CDlgNtac::OnPreIstNtac()
{
	if(!UpdateData())
	{
		return;
	}
	int nUpbound = m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vNTAC.size();
	CString str;
	
	if ( (m_nIndex > nUpbound) ||(m_nIndex <= 0) )
	{
		return;
	}
	if(m_bActive && m_strName.IsEmpty())
	{
		AfxMessageBox(_T("Setting name can not be empty if it is acitve!"));
		GetDlgItem(IDC_EDT_SETTING_NAME)->SetFocus();
		return;
	}
	SaveCurNtac();
	
	m_nCurIndex = m_nIndex-1;
	if(nUpbound < NTAC_MAX_NUM)
	{
		str.Format(_T("Do you want to insert one before the page %d?"),m_nIndex);
		if(AfxMessageBox(str,MB_YESNO) == IDYES)
		{
			NTAC_T ntac;
			ntac.Init();
			m_Parser.m_vNTAC.insert( m_Parser.m_vNTAC.begin()+ m_nIndex -1, ntac);
			nUpbound ++;
			if(nUpbound == NTAC_MAX_NUM)
			{
				GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
			}
		}
	}

	ShowCurNtac();
	if(m_nCurIndex == (nUpbound-1) && m_bViewOnly)
	{
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	}
	
	if(m_nCurIndex > 0)
	{
		GetDlgItem(IDC_BTN_PREV)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_PREV)->EnableWindow(FALSE);
	}
	
}