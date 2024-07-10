// DlgByte.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgByte.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgByte dialog


CDlgByte::CDlgByte(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgByte::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgByte)
		// NOTE: the ClassWizard will add member initialization here
	m_strByte0 = _T("0");
	m_strByte1 = _T("0");
	m_strByte2 = _T("0");
	m_strByte3 = _T("0");
	//}}AFX_DATA_INIT
	m_uByte0 = 0;
    m_uByte1 = 0;
    m_uByte2 = 0;
    m_uByte3 = 0;
	m_wValue = 0;
}


void CDlgByte::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgByte)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_EDIT_BYTE0, m_strByte0);
	DDV_MaxChars(pDX, m_strByte0, 1);
	DDX_Text(pDX, IDC_EDIT_BYTE1, m_strByte1);
	DDV_MaxChars(pDX, m_strByte1, 1);
	DDX_Text(pDX, IDC_EDIT_BYTE2, m_strByte2);
	DDV_MaxChars(pDX, m_strByte2, 1);
	DDX_Text(pDX, IDC_EDIT_BYTE3, m_strByte3);
	DDV_MaxChars(pDX, m_strByte3, 1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgByte, CDialog)
	//{{AFX_MSG_MAP(CDlgByte)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgByte message handlers

void CDlgByte::OnOK() 
{
	// TODO: Add extra validation here
	GetValue(m_wValue);
	
	CDialog::OnOK();
}

void CDlgByte::InitValue(WORD wValue)
{
	m_uByte0 = wValue & 0xf;
	m_uByte1 = (wValue & 0xf0) >> 4;
	m_uByte2 = (wValue & 0xf00) >> 8;
	m_uByte3 = (wValue & 0xf000) >> 12;

	m_strByte0.Format(_T("%x"), m_uByte0);
	m_strByte1.Format(_T("%x"), m_uByte1);
	m_strByte2.Format(_T("%x"), m_uByte2);
	m_strByte3.Format(_T("%x"), m_uByte3);

	UpdateData(FALSE);
}

void CDlgByte::GetValue(WORD &wValue)
{
	UpdateData();
#ifdef _UNICODE
	char szTmp[16] = {0};
	WideCharToMultiByte(CP_ACP,0,m_strByte0,-1,szTmp,16,NULL,NULL);
	m_uByte0 = strtol( szTmp,NULL,16 );
	WideCharToMultiByte(CP_ACP,0,m_strByte1,-1,szTmp,16,NULL,NULL);
	m_uByte1 = strtol( szTmp,NULL,16 );
	WideCharToMultiByte(CP_ACP,0,m_strByte2,-1,szTmp,16,NULL,NULL);
	m_uByte2 = strtol( szTmp,NULL,16 );
	WideCharToMultiByte(CP_ACP,0,m_strByte3,-1,szTmp,16,NULL,NULL);
	m_uByte3 = strtol( szTmp,NULL,16 );
#else
	m_uByte0 = strtol( m_strByte0,NULL,16 );
	m_uByte1 = strtol( m_strByte1,NULL,16 );
	m_uByte2 = strtol( m_strByte2,NULL,16 );
	m_uByte3 = strtol( m_strByte3,NULL,16 );
#endif

	wValue = wValue & 0x0;
	wValue |= m_uByte0;
	wValue |= (m_uByte1 << 4);
	wValue |= (m_uByte2 << 8);
	wValue |= (m_uByte3 << 12);

}

void CDlgByte::SetValue(WORD wValue)
{
	m_wValue = wValue;
}

void CDlgByte::FetchValue(WORD &wValue)
{
	wValue = m_wValue;
}

BOOL CDlgByte::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitValue(m_wValue);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
