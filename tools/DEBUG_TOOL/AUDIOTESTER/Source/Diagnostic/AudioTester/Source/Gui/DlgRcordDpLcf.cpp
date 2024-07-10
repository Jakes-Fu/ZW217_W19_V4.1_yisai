// DlgRcordDpLcf.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgRcordDpLcf.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRcordDpLcf dialog


CDlgRcordDpLcf::CDlgRcordDpLcf(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRcordDpLcf::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRcordDpLcf)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBDPLcfSw = 0;
	m_nDPLcfFpL = 0;
	m_nDPLcfFpR = 0;
	m_nDPLcfGainL = 0;
	m_nDPLcfGainR = 0;
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
}


void CDlgRcordDpLcf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRcordDpLcf)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_CBIndex(pDX, IDC_COMBO_DP_LCF_SW, m_nCBDPLcfSw);
	DDX_Text(pDX, IDC_EDIT_DP_LCF_FP_L, m_nDPLcfFpL);
	DDV_MinMaxInt(pDX, m_nDPLcfFpL, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_DP_LCF_FP_R, m_nDPLcfFpR);
	DDV_MinMaxInt(pDX, m_nDPLcfFpR, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_DP_LCF_GAIN_L, m_nDPLcfGainL);
	DDV_MinMaxInt(pDX, m_nDPLcfGainL, -32768, 6);
	DDX_Text(pDX, IDC_EDIT_DP_LCF_GAIN_R, m_nDPLcfGainR);
	DDV_MinMaxInt(pDX, m_nDPLcfGainR, -32768, 6);
	DDX_Control(pDX, IDC_STATIC_DP_LCF, m_stcGraph);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRcordDpLcf, CDialog)
	//{{AFX_MSG_MAP(CDlgRcordDpLcf)
	ON_EN_CHANGE(IDC_EDIT_DP_LCF_FP_L, OnChangeEditDpLcfFpL)
	ON_EN_CHANGE(IDC_EDIT_DP_LCF_FP_R, OnChangeEditDpLcfFpR)
	ON_EN_CHANGE(IDC_EDIT_DP_LCF_GAIN_L, OnChangeEditDpLcfGainL)
	ON_EN_CHANGE(IDC_EDIT_DP_LCF_GAIN_R, OnChangeEditDpLcfGainR)
	ON_CBN_SELCHANGE(IDC_COMBO_DP_LCF_SW, OnSelchangeComboDpLcfSw)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRcordDpLcf message handlers

BOOL CDlgRcordDpLcf::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitValue(m_eq_data);
	m_stcGraph.SetValue(m_nCBDPLcfSw, m_nDPLcfFpL, m_nDPLcfFpR, m_eq_data.extend[17], m_eq_data.extend[18]);
	m_stcGraph.Invalidate();

	m_bInit = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRcordDpLcf::OnOK() 
{
	// TODO: Add extra validation here
	GetValue(m_eq_data);
	
	CDialog::OnOK();
}

void CDlgRcordDpLcf::SetValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&m_eq_data, &eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRcordDpLcf::FetchValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&eq_data, &m_eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRcordDpLcf::InitValue(EQ_STRUCT_G_T& eq_data)
{
	m_nCBDPLcfSw = (eq_data.extend[0] & 0x4) >> 2;
	m_nDPLcfFpL = eq_data.extend[15];
	m_nDPLcfFpR = eq_data.extend[16];
	int nValue = eq_data.extend[17]; 
	if(nValue == 0)
	{
		nValue = 1;
	}
	m_nDPLcfGainL = (double)(20 * (double)log10((double)nValue/(double)16384));
	m_nDPLcfGainL = (double)((short)(m_nDPLcfGainL * 10) / 10.0);
	m_nDPLcfGainL = (m_nDPLcfGainL > 6) ? 6 : m_nDPLcfGainL;
	m_nDPLcfGainL = (m_nDPLcfGainL < -32768) ? -32768 : m_nDPLcfGainL;

	nValue = eq_data.extend[18]; 
	if(nValue == 0)
	{
		nValue = 1;
	}
	m_nDPLcfGainR = (double)(20 * (double)log10((double)nValue/(double)16384));
	m_nDPLcfGainR = (double)((short)(m_nDPLcfGainR * 10) / 10.0);
	m_nDPLcfGainR = (m_nDPLcfGainR > 6) ? 6 : m_nDPLcfGainR;
	m_nDPLcfGainR = (m_nDPLcfGainR < -32768) ? -32768 : m_nDPLcfGainR;
	
	m_nDPLcfGainLBak = m_nDPLcfGainL;
	m_nDPLcfGainRBak = m_nDPLcfGainR;
	
	UpdateData(FALSE);
}

void CDlgRcordDpLcf::GetValue(EQ_STRUCT_G_T& eq_data)
{
	UpdateData();
	int n = eq_data.extend[0];
	n &= 0xfffb;
	n |= (m_nCBDPLcfSw << 2);
	eq_data.extend[0] = n;
	
	eq_data.extend[15] = m_nDPLcfFpL;
	eq_data.extend[16] = m_nDPLcfFpR;
	if(m_nDPLcfGainL != m_nDPLcfGainLBak)
	{
		eq_data.extend[17] = ceilf(((double)pow(10, m_nDPLcfGainL/20.0)) * 16384.0);
	}
	if(m_nDPLcfGainR != m_nDPLcfGainRBak)
	{
		eq_data.extend[18] = ceilf(((double)pow(10, m_nDPLcfGainR/20.0)) * 16384.0);
	}
	
}

void CDlgRcordDpLcf::ReflashGraph()
{
	if(!UpdateData())
	{
		return;
	}
	if(m_nDPLcfGainL != m_nDPLcfGainLBak)
	{
		m_eq_data.extend[17] = ceilf(((double)pow(10, m_nDPLcfGainL/20.0)) * 16384.0);
	}
	if(m_nDPLcfGainR != m_nDPLcfGainRBak)
	{
		m_eq_data.extend[18] = ceilf(((double)pow(10, m_nDPLcfGainR/20.0)) * 16384.0);
	}
	m_stcGraph.SetValue(m_nCBDPLcfSw, m_nDPLcfFpL, m_nDPLcfFpR, m_eq_data.extend[17], m_eq_data.extend[18]);
	m_stcGraph.Invalidate(FALSE);
}

void CDlgRcordDpLcf::OnChangeEditDpLcfFpL() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_DP_LCF_FP_L)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
	
}

void CDlgRcordDpLcf::OnChangeEditDpLcfFpR() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_DP_LCF_FP_R)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
	
}

void CDlgRcordDpLcf::OnChangeEditDpLcfGainL() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_DP_LCF_GAIN_L)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
	
}

void CDlgRcordDpLcf::OnChangeEditDpLcfGainR() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_DP_LCF_GAIN_R)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgRcordDpLcf::OnSelchangeComboDpLcfSw() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_bInit)
	{
		ReflashGraph();
	}
}

HBRUSH CDlgRcordDpLcf::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_L )
    {
        return CreateSolidBrush( RGB( 0,0,255) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_R )
    {
        return CreateSolidBrush( RGB( 255,0,0 ) );
    }
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgRcordDpLcf::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN) 
	{
		if(!UpdateData())
		{
			return FALSE;
		}
		if(m_bInit)
		{
			ReflashGraph();
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
