// DlgCNG.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgCNG.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCNG dialog


CDlgCNG::CDlgCNG(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCNG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCNG)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBIndexCNGSW = 0;
	m_nCBIndexGain = 0;
	//}}AFX_DATA_INIT
	m_fCNGGain = 0;
}


void CDlgCNG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCNG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_CBIndex(pDX, IDC_COMBO_CNG_SW, m_nCBIndexCNGSW);
	DDX_CBIndex(pDX, IDC_COMBO_CNG_GAIN, m_nCBIndexGain);
	DDX_Control(pDX, IDC_COMBO_CNG_GAIN, m_boxCNGGain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCNG, CDialog)
	//{{AFX_MSG_MAP(CDlgCNG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCNG message handlers

BOOL CDlgCNG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	DWORD dwValue = m_dwCNGGain;
	if(dwValue == 0)
	{
		dwValue = 1;
	}
	m_fCNGGain = (double)(20 * (double)log10((double)dwValue/(double)32768.0));
	m_fCNGGain = (double)((short)floorf(m_fCNGGain * 10) / 10.0);
	
	int nGain = (int)m_fCNGGain;
	CString strText = _T("");
	strText.Format(_T("%d"), nGain);
	m_nCBIndexGain = m_boxCNGGain.FindString(0, strText);

	m_nCBIndexGainBak = m_nCBIndexGain;
	m_nCBIndexCNGSW = m_dwCNGSW;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCNG::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	m_dwCNGSW = m_nCBIndexCNGSW;

	if(m_nCBIndexGain != m_nCBIndexGainBak)
	{
		CString strText;
		m_boxCNGGain.GetLBText(m_nCBIndexGain, strText);
#ifdef _UNICODE
		char szTmp[16] = {0};
		WideCharToMultiByte(CP_ACP,0,strText,-1,szTmp,16,NULL,NULL);
		int nGain = atoi(szTmp);
#else
		int nGain = atoi( (LPSTR)(LPCTSTR)strText );   
#endif  
		m_dwCNGGain = ((double)pow(10, nGain/20.0)) * 32768.0;
	}

	
	CDialog::OnOK();
}
