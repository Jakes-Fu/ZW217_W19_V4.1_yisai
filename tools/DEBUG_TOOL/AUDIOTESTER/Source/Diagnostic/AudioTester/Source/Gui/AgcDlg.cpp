// AgcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "AgcDlg.h"

#include "BMPCtrlWithSpecRect.h"
#include <math.h>

#include "AudioTesterSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAgcDlg dialog


CAgcDlg::CAgcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAgcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAgcDlg)
	m_bWriteImmediately = FALSE;
	m_nSwitch = -1;
	m_nALCDPthreshold = 0;
	m_nALCcprsThreshold = 0;
	m_nALCcprsLimit = 0;
	m_nALCexpdThreshold = 0;
	m_nALCexpdLimit = 0;
	m_nALCcprsRatio = 0;
	m_nALCexpdRatio = 0;
	//}}AFX_DATA_INIT
    
    m_pArea = NULL;
	m_bAudio4 = FALSE;
	m_bAudio41 = FALSE;
}


void CAgcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgcDlg)
	DDX_Check(pDX, IDC_CHECK_WRITE_IMMEDIATELY, m_bWriteImmediately);
	DDX_CBIndex(pDX, IDC_COMBO_AGC, m_nSwitch);
	DDX_Text(pDX, IDC_EDIT_ALC_DP_THRESHOLD, m_nALCDPthreshold);
	DDV_MinMaxInt(pDX, m_nALCDPthreshold, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_ALC_COMPRESS_THRESHOLD, m_nALCcprsThreshold);
	DDV_MinMaxInt(pDX, m_nALCcprsThreshold, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_ALC_COMPRESS_LIMIT, m_nALCcprsLimit);
	DDV_MinMaxInt(pDX, m_nALCcprsLimit, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_ALC_EXPAND_THRESHOLD, m_nALCexpdThreshold);
	DDV_MinMaxInt(pDX, m_nALCexpdThreshold, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_ALC_EXPAND_LIMIT, m_nALCexpdLimit);
	DDV_MinMaxInt(pDX, m_nALCexpdLimit, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_ALC_COMPRESS_RATIO, m_nALCcprsRatio);
	//DDV_MinMaxInt(pDX, m_nALCcprsRatio, 0, 45);
	DDX_Text(pDX, IDC_EDIT_ALC_EXPAND_RATIO, m_nALCexpdRatio);
	//DDV_MinMaxInt(pDX, m_nALCexpdRatio, 9, 45);
	DDX_Control(pDX, IDC_STATIC_ALC_GRAPH, m_stcGraph);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAgcDlg, CDialog)
	//{{AFX_MSG_MAP(CAgcDlg)
	ON_EN_CHANGE(IDC_EDIT_ALC_EXPAND_LIMIT, OnChangeEditAlcExpandLimit)
	ON_EN_CHANGE(IDC_EDIT_ALC_EXPAND_THRESHOLD, OnChangeEditAlcExpandThreshold)
	ON_EN_CHANGE(IDC_EDIT_ALC_COMPRESS_THRESHOLD, OnChangeEditAlcCompressThreshold)
	ON_EN_CHANGE(IDC_EDIT_ALC_COMPRESS_LIMIT, OnChangeEditAlcCompressLimit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAgcDlg message handlers

BOOL CAgcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_nSwitch = ((SpecialRectOfAgc*)m_pArea)->m_iValue;
	//teana hu 2011.02.14
	m_bAudio4 = ((SpecialRectOfAgc*)m_pArea)->m_bAudio4;
	//teana hu 2011.03.24
	m_bAudio41 = ((SpecialRectOfAgc*)m_pArea)->m_bAudio41;
	if(m_bAudio4)
	{
		SetWindowText(_T("ALC Switch"));
		m_nALCDPthreshold = ((SpecialRectOfAgc*)m_pArea)->m_nALCDPthreshold;
		GetDlgItem(IDC_STATIC_AGC)->SetWindowText(_T("ALC Switch"));
		GetDlgItem(IDC_STATIC_ALC_COMPRESS_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_COMPRESS_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_COMPRESS_LIMIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_COMPRESS_LIMIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_EXPAND_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_EXPAND_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_EXPAND_LIMIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_EXPAND_LIMIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_COMPRESS_RATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_COMPRESS_RATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_EXPAND_RATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_EXPAND_RATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_GRAPH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_DESP)->ShowWindow(SW_HIDE);
	}
	else if(m_bAudio41)
	{
		SetWindowText(_T("ALC Switch"));
		GetDlgItem(IDC_STATIC_AGC)->SetWindowText(_T("ALC Switch"));
		GetDlgItem(IDC_STATIC_ALC_DP_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_DP_THRESHOLD)->ShowWindow(SW_HIDE);
		m_nALCcprsThreshold = ((SpecialRectOfAgc*)m_pArea)->m_nALCcprsThreshold;
		m_nALCcprsLimit = ((SpecialRectOfAgc*)m_pArea)->m_nALCcprsLimit;
		m_nALCexpdThreshold = ((SpecialRectOfAgc*)m_pArea)->m_nALCexpdThreshold;
		m_nALCexpdLimit = ((SpecialRectOfAgc*)m_pArea)->m_nALCexpdLimit;

		//teana hu 2011.05.23
		int n = ((SpecialRectOfAgc*)m_pArea)->m_nALCcprsRatio;
		double m = 1 - n/32768.0;
		double x = atan(m);
		m_nALCcprsRatio = (x * 180.0 ) / 3.1415926;
		n = ((SpecialRectOfAgc*)m_pArea)->m_nALCexpdRatio;
		m = 1 / (n / 4096.0 + 1);
		x = atan(m);
		m_nALCexpdRatio = (x * 180.0 ) / 3.1415926;
		//
	}
	else
	{
		SetWindowText(_T("AGC Switch"));
		GetDlgItem(IDC_STATIC_AGC)->SetWindowText(_T("AGC Switch"));
		GetDlgItem(IDC_STATIC_ALC_DP_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_DP_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_COMPRESS_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_COMPRESS_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_COMPRESS_LIMIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_COMPRESS_LIMIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_EXPAND_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_EXPAND_THRESHOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_EXPAND_LIMIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_EXPAND_LIMIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_COMPRESS_RATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_COMPRESS_RATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_EXPAND_RATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALC_EXPAND_RATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_GRAPH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALC_DESP)->ShowWindow(SW_HIDE);
	}
	
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
	CString strAll("All");
	pSheet->m_CtrlStatus.RefreshControl( strAll,IDD_DIALOG_AGC,this );

	//teana hu 2011.06.14
	if(m_bAudio41)
	{
		ALC_CURVE_DATA data;
		data.y1 = m_nALCexpdLimit/10;
		data.y2 = m_nALCexpdThreshold/10;
		data.y3 = m_nALCcprsThreshold/10;
		data.y4 = m_nALCcprsLimit/10;
		m_stcGraph.SetData(data);
	}
	
	//

    UpdateData( FALSE );
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAgcDlg::OnOK() 
{
    if( !UpdateData() )
    {
        return;
    }    
    
    SpecialRect::m_bImmediateWrite = m_bWriteImmediately;    
    ((SpecialRectOfAgc*)m_pArea)->m_iValue = m_nSwitch;
	//teana hu 2011.02.14
	if(m_bAudio4)
	{
		((SpecialRectOfAgc*)m_pArea)->m_nALCDPthreshold = m_nALCDPthreshold;
	}
	else if(m_bAudio41)
	{
		((SpecialRectOfAgc*)m_pArea)->m_nALCcprsThreshold = m_nALCcprsThreshold;
		((SpecialRectOfAgc*)m_pArea)->m_nALCcprsLimit = m_nALCcprsLimit;
		((SpecialRectOfAgc*)m_pArea)->m_nALCexpdThreshold = m_nALCexpdThreshold;
		((SpecialRectOfAgc*)m_pArea)->m_nALCexpdLimit = m_nALCexpdLimit;
		//teana hu 2011.05.23
		double n = tan(m_nALCcprsRatio * 3.1415926 / 180.0);
		((SpecialRectOfAgc*)m_pArea)->m_nALCcprsRatio = (1 - n) * 32768.0;
		n = tan(m_nALCexpdRatio * 3.1415926 / 180.0);
		if(n != 0)
		{
			((SpecialRectOfAgc*)m_pArea)->m_nALCexpdRatio = (1/n - 1) * 4096.0;
		}
		//
	}
	//
	CDialog::OnOK();
}

void CAgcDlg::OnChangeEditAlcExpandLimit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bAudio41)
	{
		CString strtext;
		GetDlgItem(IDC_EDIT_ALC_EXPAND_LIMIT)->GetWindowText(strtext);
		if(strtext == _T("-"))
		{
			return;
		}
		
		RefeshGraph();
	}
	
}

void CAgcDlg::OnChangeEditAlcExpandThreshold() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bAudio41)
	{
		CString strtext;
		GetDlgItem(IDC_EDIT_ALC_EXPAND_THRESHOLD)->GetWindowText(strtext);
		if(strtext == _T("-"))
		{
			return;
		}
		
		RefeshGraph();
	}
	
}

void CAgcDlg::OnChangeEditAlcCompressThreshold() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bAudio41)
	{
		CString strtext;
		GetDlgItem(IDC_EDIT_ALC_COMPRESS_THRESHOLD)->GetWindowText(strtext);
		if(strtext == _T("-"))
		{
			return;
		}
		
		RefeshGraph();
	}
	
}

void CAgcDlg::OnChangeEditAlcCompressLimit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bAudio41)
	{
		CString strtext;
		GetDlgItem(IDC_EDIT_ALC_COMPRESS_LIMIT)->GetWindowText(strtext);
		if(strtext == _T("-"))
		{
			return;
		}
		
		RefeshGraph();
	}
	
}

void CAgcDlg::RefeshGraph()
{
	UpdateData();
	ALC_CURVE_DATA data;
	data.y1 = m_nALCexpdLimit/10;
	data.y2 = m_nALCexpdThreshold/10;
	data.y3 = m_nALCcprsThreshold/10;
	data.y4 = m_nALCcprsLimit/10;
	m_stcGraph.SetData(data);

	m_stcGraph.Invalidate(FALSE);
}