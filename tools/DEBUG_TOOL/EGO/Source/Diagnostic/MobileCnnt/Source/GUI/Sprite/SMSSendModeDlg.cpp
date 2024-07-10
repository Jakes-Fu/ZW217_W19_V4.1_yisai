// SMSSendModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "SMSSendModeDlg.h"
#include "IEtion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSSendModeDlg dialog


CSMSSendModeDlg::CSMSSendModeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSSendModeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSSendModeDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_strCurMode = _T("");
	//}}AFX_DATA_INIT
}


void CSMSSendModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSSendModeDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_SMS_MODE, m_modeBox);
	DDX_Text(pDX, IDC_EDIT_CUR_SMS_MODE , m_strCurMode);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSSendModeDlg, CDialog)
	//{{AFX_MSG_MAP(CSMSSendModeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSSendModeDlg message handlers

BOOL CSMSSendModeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ModifyStyleEx(0, WS_EX_TOOLWINDOW);

	CenterWindow();	

	for(int i = 0; i < MODE_NUM; i++)
	{
		m_modeBox.AddString(SEND_MODE[i]);
	}
	m_modeBox.SetCurSel(0);

	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(SMS_SEND_MODE_CHECK, 0, NULL, 0);
		if(lpRet != NULL)
		{
			strcpy(szRet, lpRet);
		}
		
		if(strstr(szRet, "ERROR") == NULL)
		{
			CString strText = szRet;
			int nIndex = strText.Find(_T("+CGSMS:"));
			if(nIndex != -1)
			{
				UINT uIndex = 0;
#ifdef _UNICODE
				wscanf(strText.Right(1), _T("%d"), &uIndex);
#else
				sscanf(strText.Right(1), _T("%d"), &uIndex);
#endif
				
				m_strCurMode = SEND_MODE[uIndex];
			}
		}
		
	}
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSMSSendModeDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	int nIndex = m_modeBox.GetCurSel();
	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};
		
		TCHAR szPara[MAX_PATH] = {0};
#ifdef _UNICODE
		wsprintf(szPara, _T("%d"), nIndex);
#else
		sprintf(szPara, _T("%d"), nIndex);
#endif	
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(SMS_SEND_MODE_SET, 0, (LPVOID)szPara, _tcslen(szPara));
		if(lpRet != NULL)
		{
			strcpy(szRet, lpRet);
		}
		
		if(strstr(szRet, "OK") != NULL)
		{
			AfxMessageBox(_T("设置成功"));
		}
		if(strstr(szRet, "ERROR") != NULL)
		{
			AfxMessageBox(_T("设置失败"));
		}
	}

}
