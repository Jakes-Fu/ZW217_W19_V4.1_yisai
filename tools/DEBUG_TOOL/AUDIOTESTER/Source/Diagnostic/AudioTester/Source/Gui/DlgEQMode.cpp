// DlgEQMode.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgEQMode.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEQMode dialog


CDlgEQMode::CDlgEQMode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEQMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEQMode)
		// NOTE: the ClassWizard will add member initialization here
	m_fEQGain = 0.0;
	m_nSampleRate = 44100;
	m_nFilterType = 0;
	//}}AFX_DATA_INIT
	m_nSoundEqVer = sound_eq_ver_3;
}


void CDlgEQMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEQMode)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_EDT_EQ_GAIN, m_fEQGain);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_RATE, m_nSampleRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEQMode, CDialog)
	//{{AFX_MSG_MAP(CDlgEQMode)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEQMode message handlers

BOOL CDlgEQMode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(&rect);
	
	rect.top += 40;

	m_DlgEQMode2.Create(IDD_DIALOG_EQ_MODE_2, NULL);	
	m_DlgEQMode2.SetParent(this); // this is the key step, must be set after change style	
	m_DlgEQMode2.SetWindowPos(&wndTop,rect.left,rect.top, rect.Width(),rect.Height(),SWP_DRAWFRAME);
	//m_DlgEQMode2.ShowWindow(SW_SHOW);

	m_DlgEQMode3.Create(IDD_DIALOG_EQ_MODE_3, NULL);
	m_DlgEQMode3.SetParent(this); // this is the key step, must be set after change style	
	m_DlgEQMode3.SetWindowPos(&wndTop,rect.left,rect.top, rect.Width(),rect.Height(),SWP_DRAWFRAME);
	//m_DlgEQMode3.ShowWindow(SW_HIDE);

	UpdateData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEQMode::SetPara(EQ_MODE_NV_PARAM_T EQMode, short eqControl,BUTTERWORTH_LCF_T lcf_data )
{
	int nValue = EQMode.agc_in_gain;
	if(nValue == 0)
	{
		nValue = 1;	
	}
	m_fEQGain = 20 * (double)log10((double)(nValue/(double)4096.0));
	m_fEQGain = (short)(m_fEQGain * 10) / 10.0;
	m_fEQGain = m_fEQGain > 18.0 ? 18.0 :m_fEQGain;

	m_nSoundEqVer = GET_SOUND_EQ_VER( eqControl );
	if( sound_eq_ver_3 == m_nSoundEqVer )
	{
		m_nFilterType = sound_eq_mode_f1f1;
	}
	else
	{
		m_nFilterType = (eqControl & 0x700) >> 8;
	}

	//teana hu 2011.06.16
// 	if(m_nFilterType == sound_eq_mode_f1f1 )
// 	{
// 		m_DlgEQMode3.ShowWindow(SW_SHOW);
// 	}
// 	else
// 	{
// 		m_DlgEQMode2.ShowWindow(SW_SHOW);
// 	}

	m_DlgEQMode2.SetPara(EQMode,lcf_data);
	m_DlgEQMode3.SetPara(EQMode,eqControl);
	//
	
	UpdateData(FALSE);
}

BOOL CDlgEQMode::GetPara(EQ_MODE_NV_PARAM_T &EQMode, short &eqControl,BUTTERWORTH_LCF_T& lcf_data )
{
    if(!UpdateData())
	{
		return FALSE;
	}
	EQMode.agc_in_gain = (double)pow(10, m_fEQGain/20.0) * 4096;

	if( sound_eq_ver_4 == m_nSoundEqVer )
	{
		int nFilterType = (eqControl & 0x700) >> 8;
		if( sound_eq_mode_f1f1 == nFilterType )
		{
			if( !m_DlgEQMode2.GetPara(EQMode,lcf_data) )
			{
				return FALSE;
			}
			if( !m_DlgEQMode3.GetPara(EQMode) )
			{
				return FALSE;
			}
		}
		else
		{
			if( !m_DlgEQMode3.GetPara(EQMode) )
			{
				return FALSE;
			}
			if( !m_DlgEQMode2.GetPara(EQMode,lcf_data) )
			{
				return FALSE;
			}
		}		
	}
	else
	{
		if( !m_DlgEQMode3.GetPara(EQMode) )
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgEQMode::OnDestroy() 
{
	m_DlgEQMode2.DestroyWindow();
	m_DlgEQMode3.DestroyWindow();

	CDialog::OnDestroy();
}

BOOL CDlgEQMode::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message)
    {
        // Process when receive ENTER
        if (VK_RETURN == pMsg->wParam || pMsg->wParam == VK_ESCAPE)
        {
            return TRUE;
        }
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgEQMode::ChangeFilterType( UINT nFilterType ) 
{
	m_nFilterType = nFilterType;

	if(m_nFilterType == 0)
	{
		m_DlgEQMode2.ShowWindow(SW_HIDE);
		m_DlgEQMode3.ShowWindow(SW_SHOW);
	}
	else if(m_nFilterType == 1)
	{
		m_DlgEQMode2.ShowWindow(SW_SHOW);
		m_DlgEQMode3.ShowWindow(SW_HIDE);
	}
}

void CDlgEQMode::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if( bShow )
	{
		m_DlgEQMode2.ParentShow();
		m_DlgEQMode3.ParentShow();
	}
	
}
