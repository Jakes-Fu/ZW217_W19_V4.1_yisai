// DlgRecorDpAlc.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgRecorDpAlc.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRecorDpAlc dialog


CDlgRecorDpAlc::CDlgRecorDpAlc(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecorDpAlc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRecorDpAlc)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBDPZcSw = 0;
	m_nCBDPSw = 0;
	m_nDPSdelay = 0;
	m_nDPLimitUp = 0;
	m_nDPLimitDown = 0;
	m_nCompressorThreshold = 0;
	m_nCompressorRatio = 0;
	m_nCompressorAttack = 0;
	m_nCompressorHold = 0;
	m_nCompressorRelease = 0;
	m_nExpanderThreshold = 0;
	m_nExpanderRatio = 0;
	m_nExpanderAttack = 0;
	m_nExpanderHold = 0;
	m_nExpanderRelease = 0;
	//}}AFX_DATA_INIT
}


void CDlgRecorDpAlc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRecorDpAlc)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_CBIndex(pDX, IDC_COMBO_DP_SW, m_nCBDPSw);
	DDX_CBIndex(pDX, IDC_COMBO_DP_ZC_SW, m_nCBDPZcSw);
	DDX_Text(pDX, IDC_EDIT_DP_SDELAY, m_nDPSdelay);
	DDV_MinMaxInt(pDX, m_nDPSdelay, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_DP_LIMIT_UP, m_nDPLimitUp);
	DDV_MinMaxInt(pDX, m_nDPLimitUp, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_DP_LIMIT_DOWN, m_nDPLimitDown);
	DDV_MinMaxInt(pDX, m_nDPLimitDown, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_COMPRESSOR_THRESHOLD, m_nCompressorThreshold);
	DDV_MinMaxInt(pDX, m_nCompressorThreshold, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_COMPRESSOR_RATIO, m_nCompressorRatio);
	DDV_MinMaxInt(pDX, m_nCompressorRatio, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_COMPRESSOR_ATTACK, m_nCompressorAttack);
	DDV_MinMaxInt(pDX, m_nCompressorAttack, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_COMPRESSOR_HOLD, m_nCompressorHold);
	DDV_MinMaxInt(pDX, m_nCompressorHold, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_COMPRESSOR_RELEASE, m_nCompressorRelease);
	DDV_MinMaxInt(pDX, m_nCompressorRelease, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_EXPANDER_THRESHOLD, m_nExpanderThreshold);
	DDV_MinMaxInt(pDX, m_nExpanderThreshold, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_EXPANDER_RATIO, m_nExpanderRatio);
	DDV_MinMaxInt(pDX, m_nExpanderRatio, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_EXPANDER_ATTACK, m_nExpanderAttack);
	DDV_MinMaxInt(pDX, m_nExpanderAttack, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_EXPANDER_HOLD, m_nExpanderHold);
	DDV_MinMaxInt(pDX, m_nExpanderHold, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_EXPANDER_RELEASE, m_nExpanderRelease);
	DDV_MinMaxInt(pDX, m_nExpanderRelease, -32768, 32767);
	DDX_Control(pDX, IDC_STATIC_GRAPH_1, m_stcGraph1);
	DDX_Control(pDX, IDC_STATIC_GRAPH_2, m_stcGraph2);
	DDX_Control(pDX, IDC_STATIC_GRAPH_3, m_stcGraph3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRecorDpAlc, CDialog)
	//{{AFX_MSG_MAP(CDlgRecorDpAlc)
	ON_EN_CHANGE(IDC_EDIT_DP_LIMIT_UP, OnChangeEditDpLimitUp)
	ON_EN_CHANGE(IDC_EDIT_DP_LIMIT_DOWN, OnChangeEditDpLimitDown)
	ON_EN_CHANGE(IDC_EDIT_COMPRESSOR_THRESHOLD, OnChangeEditCompressorThreshold)
	ON_EN_CHANGE(IDC_EDIT_EXPANDER_THRESHOLD, OnChangeEditExpanderThreshold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRecorDpAlc message handlers

CScrollBar* CDlgRecorDpAlc::GetScrollBarCtrl(int nBar) const
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::GetScrollBarCtrl(nBar);
}

BOOL CDlgRecorDpAlc::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitValue(m_eq_data);

	ALC_CURVE_DATA data;
	data.y1 = m_nDPLimitDown;
	data.y2 = m_nExpanderThreshold;
	data.y3 = m_nCompressorThreshold;
	data.y4 = m_nDPLimitUp;
	m_stcGraph1.SetData(data);
	m_stcGraph2.m_bCompressor = TRUE;
	m_stcGraph2.Invalidate();
	m_stcGraph2.m_bCompressor = FALSE;
	m_stcGraph3.Invalidate();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRecorDpAlc::OnOK() 
{
	// TODO: Add extra validation here
	GetValue(m_eq_data);
	
	CDialog::OnOK();
}

void CDlgRecorDpAlc::SetValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&m_eq_data, &eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRecorDpAlc::FetchValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&eq_data, &m_eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRecorDpAlc::InitValue(EQ_STRUCT_G_T& eq_data)
{
	m_nCBDPSw = eq_data.extend[0] & 0x1;
	m_nCBDPZcSw = (eq_data.extend[0] & 0x2) >> 1;

	m_nDPSdelay = eq_data.extend[2];
	m_nDPLimitUp = eq_data.extend[3];
	m_nDPLimitDown = eq_data.extend[4];
	m_nCompressorThreshold = eq_data.extend[5];
	m_nCompressorRatio = eq_data.extend[6];
	m_nCompressorAttack = eq_data.extend[7];
	m_nCompressorHold = eq_data.extend[8];
	m_nCompressorRelease = eq_data.extend[9];
	m_nExpanderThreshold = eq_data.extend[10];
	m_nExpanderRatio = eq_data.extend[11];
	m_nExpanderAttack = eq_data.extend[12];
	m_nExpanderHold = eq_data.extend[13];
	m_nExpanderRelease = eq_data.extend[14];

	int n = m_nCompressorRatio;
	double m = 1 - n/32768.0;
	double x = atan(m);
	m_nCompressorRatio = (x * 180.0 ) / 3.1415926;
	n = m_nExpanderRatio;
	m = 1 - n/32768.0;
	x = atan(m);
	m_nExpanderRatio = (x * 180.0 ) / 3.1415926;

	
	UpdateData(FALSE);
}

void CDlgRecorDpAlc::GetValue(EQ_STRUCT_G_T& eq_data)
{
	UpdateData();
	int n = eq_data.extend[0];
	n &= 0xfffc;
	n |= m_nCBDPSw;
	n |= (m_nCBDPZcSw << 1);
	eq_data.extend[0] = n;
	
	eq_data.extend[2] = m_nDPSdelay;
	eq_data.extend[3] = m_nDPLimitUp;
	eq_data.extend[4] = m_nDPLimitDown;
	eq_data.extend[5] = m_nCompressorThreshold;
	//eq_data.extend[6] = m_nCompressorRatio;
	eq_data.extend[7] = m_nCompressorAttack;
	eq_data.extend[8] = m_nCompressorHold;
	eq_data.extend[9] = m_nCompressorRelease;
	eq_data.extend[10] = m_nExpanderThreshold;
	//eq_data.extend[11] = m_nExpanderRatio;
	eq_data.extend[12] = m_nExpanderAttack;
	eq_data.extend[13] = m_nExpanderHold;
	eq_data.extend[14] = m_nExpanderRelease;
	
	double m = tan(m_nCompressorRatio * 3.1415926 / 180.0);
	eq_data.extend[6] = (1 - m) * 32768.0;
	m = tan(m_nExpanderRatio * 3.1415926 / 180.0);
	eq_data.extend[11] = (1 - m) * 32768.0;


}


void CDlgRecorDpAlc::RefeshGraph()
{
	UpdateData();
	ALC_CURVE_DATA data;
	data.y1 = m_nDPLimitDown;
	data.y2 = m_nExpanderThreshold;
	data.y3 = m_nCompressorThreshold;
	data.y4 = m_nDPLimitUp;
	m_stcGraph1.SetData(data);
	
	m_stcGraph1.Invalidate(FALSE);
}

void CDlgRecorDpAlc::OnChangeEditDpLimitUp() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_DP_LIMIT_UP)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	
	RefeshGraph();
	
}

void CDlgRecorDpAlc::OnChangeEditDpLimitDown() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_DP_LIMIT_DOWN)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	
	RefeshGraph();
	
}

void CDlgRecorDpAlc::OnChangeEditCompressorThreshold() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_COMPRESSOR_THRESHOLD)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	
	RefeshGraph();
	
}

void CDlgRecorDpAlc::OnChangeEditExpanderThreshold() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_EXPANDER_THRESHOLD)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	
	RefeshGraph();
	
}
