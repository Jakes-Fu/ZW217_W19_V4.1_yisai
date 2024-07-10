// StNgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "BMPCtrlWithSpecRect.h"
#include "StNgDlg.h"

#include "AudioTesterSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CStNgDlg dialog


CStNgDlg::CStNgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStNgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStNgDlg)
	m_st_ng_switch = -1;
	m_st_att = 0;
	m_st_holdc = 0;
	m_st_noise_threshold = 0;
	m_st_voice_threshold = 0;
	m_st_zcr_ceil = 0;
	m_st_zcr_threshold = 0;
	//}}AFX_DATA_INIT
	m_pArea = NULL;
}


void CStNgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStNgDlg)
	DDX_CBIndex(pDX, IDC_COMBO_ST_NG_SWITCH, m_st_ng_switch);
	DDX_Text(pDX, IDC_EDIT_ST_ATT, m_st_att);
	DDV_MinMaxInt(pDX, m_st_att, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_ST_HOLDC, m_st_holdc);
	DDV_MinMaxInt(pDX, m_st_holdc, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_ST_NOISE_THRESHOLD, m_st_noise_threshold);
	DDV_MinMaxInt(pDX, m_st_noise_threshold, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_ST_VOICE_THRESHOLD, m_st_voice_threshold);
	DDV_MinMaxInt(pDX, m_st_voice_threshold, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_ST_ZCR_CEIL, m_st_zcr_ceil);
	DDV_MinMaxInt(pDX, m_st_zcr_ceil, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_ST_ZCR_THRESHOLD, m_st_zcr_threshold);
	DDV_MinMaxInt(pDX, m_st_zcr_threshold, 0, 32767);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStNgDlg, CDialog)
	//{{AFX_MSG_MAP(CStNgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStNgDlg message handlers

BOOL CStNgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_pArea != NULL)
	{
		m_st_ng_switch = m_pArea->m_st_ng_switch;
		m_st_att = m_pArea->m_st_att;
		m_st_holdc = m_pArea->m_st_holdc;
		m_st_noise_threshold = m_pArea->m_st_noise_threshold;
		m_st_voice_threshold = m_pArea->m_st_voice_threshold;
		m_st_zcr_ceil = m_pArea->m_st_zcr_ceil;
		m_st_zcr_threshold = m_pArea->m_st_zcr_threshold;
	}

    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
	CString strAll("All");
	pSheet->m_CtrlStatus.RefreshControl( strAll,IDD_DIALOG_SIDETONE_NG,this );
	CenterWindow();
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStNgDlg::OnOK() 
{
    if (!UpdateData(TRUE))
    {
        TRACE0("UpdateData failed during dialog termination.\n");
        // the UpdateData routine will set focus to correct item
        return;
    }

	m_pArea->m_st_ng_switch = m_st_ng_switch;
	m_pArea->m_st_att = m_st_att;
	m_pArea->m_st_holdc = m_st_holdc;
	m_pArea->m_st_noise_threshold = m_st_noise_threshold;
	m_pArea->m_st_voice_threshold = m_st_voice_threshold;
	m_pArea->m_st_zcr_ceil = m_st_zcr_ceil;
	m_pArea->m_st_zcr_threshold = m_st_zcr_threshold;

    EndDialog(IDOK);
}
