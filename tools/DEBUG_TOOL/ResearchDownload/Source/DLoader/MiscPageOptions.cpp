// MiscPageOptions.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "MiscPageOptions.h"
#include "afxdialogex.h"



// CMiscPageOptions dialog

IMPLEMENT_DYNAMIC(CMiscPageOptions, CPropertyPage)

CMiscPageOptions::CMiscPageOptions()
	: CPropertyPage(CMiscPageOptions::IDD)
	, m_strTmpSelinux(_T(""))
{

}

CMiscPageOptions::~CMiscPageOptions()
{
}

void CMiscPageOptions::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SELINUX, m_comboSelinux);
}


BEGIN_MESSAGE_MAP(CMiscPageOptions, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_SELINUX, &CMiscPageOptions::OnCbnSelchangeComboSelinux)
END_MESSAGE_MAP()


// CMiscPageOptions message handlers


BOOL CMiscPageOptions::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strSelinux = m_strSelinux.Right(1);
	int nIndex = 0;
	if (strSelinux.Compare(_T("-")) == 0)
	{
		nIndex = 0;
	}
	else if (strSelinux.Compare(_T("0")) == 0)
	{
		nIndex = 1;
	}
	else if (strSelinux.Compare(_T("1")) == 0)
	{
		nIndex = 2;
	}
	m_comboSelinux.SetCurSel(nIndex);
	m_strTmpSelinux = m_strSelinux;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CMiscPageOptions::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();
	m_strSelinux = m_strTmpSelinux;

	if(!m_strIniFile.IsEmpty())
	{
		DWORD dwAttr = GetFileAttributes(m_strIniFile);
		if(MAXDWORD != dwAttr)
		{
			dwAttr &= ~FILE_ATTRIBUTE_READONLY;
			::SetFileAttributes(m_strIniFile,dwAttr);
		}
		SaveSettings(m_strIniFile);
	}

	CPropertyPage::OnOK();
}

BOOL CMiscPageOptions::LoadSettings(LPCTSTR pFileName,CString &strErrorMsg)
{
	//	UNUSED_ALWAYS(strErrorMsg);
	ASSERT(NULL != pFileName);
	if(NULL == pFileName)
	{
		strErrorMsg += _T("Configure file is empty!\n");
		return FALSE;
	}

	m_strIniFile        = pFileName;	
	TCHAR szBuf[MAX_PATH] = {0};
	memset(szBuf,0,sizeof(szBuf));
	GetPrivateProfileString(_T("Selinux"),_T("Data"),_T("Selinux:-"),szBuf,_MAX_PATH,pFileName);
	m_strSelinux = szBuf;

	return TRUE;
}

BOOL CMiscPageOptions::SaveSettings(LPCTSTR pFileName)
{
	ASSERT(NULL != pFileName);
	if(NULL == pFileName)
	{
		return FALSE;
	}

	WritePrivateProfileString( _T("Selinux"),_T("Data"),m_strSelinux,pFileName ); 


	return TRUE;
}


void CMiscPageOptions::OnCbnSelchangeComboSelinux()
{
	// TODO: Add your control notification handler code here
	CString strSelinux;
	int nSel = m_comboSelinux.GetCurSel();
	m_comboSelinux.GetLBText(nSel, strSelinux);
	int nIndex = m_strTmpSelinux.GetLength()-1;
	m_strTmpSelinux.SetAt(nIndex, strSelinux.GetBuffer()[0]);
}
