// DlgEQContent.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgEQContent.h"
#include <math.h>

#define MIN_EQ_LIMIT -42
#define MAX_EQ_LIMIT 0

#define MIN_LCF_GAIN_LR -84
#define MAX_LCF_GAIN_LR 6

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEQContent dialog


CDlgEQContent::CDlgEQContent(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEQContent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEQContent)
	m_nEQSwitch = 0;
	m_dEQLimit = 0.0;
	m_nCBFilterType = -1;
	m_fLcfGainL = 0.0;
	m_fLcfGainR = 0.0;
	//}}AFX_DATA_INIT
	m_dlimitBak = 0.0;
	m_nSoundEqVer = sound_eq_ver_3;
}


void CDlgEQContent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEQContent)
	DDX_CBIndex(pDX, IDC_COMBO_EQ_SWITCH, m_nEQSwitch);
	DDX_Text(pDX, IDC_EDT_EQ_LIMIT, m_dEQLimit);
	DDV_MinMaxDouble(pDX, m_dEQLimit, MIN_EQ_LIMIT, MAX_EQ_LIMIT);
	DDX_Control(pDX, IDC_TAB_EQ_CONTENT, m_tabCtrl);
	DDX_CBIndex(pDX, IDC_COMBO_FILTER_TYPE, m_nCBFilterType);
	DDX_Text(pDX, IDC_EDT_LCF_GAIN_L, m_fLcfGainL);
	DDV_MinMaxDouble(pDX, m_fLcfGainL, MIN_LCF_GAIN_LR, MAX_LCF_GAIN_LR);
	DDX_Text(pDX, IDC_EDT_LCF_GAIN_R, m_fLcfGainR);
	DDV_MinMaxDouble(pDX, m_fLcfGainR, MIN_LCF_GAIN_LR, MAX_LCF_GAIN_LR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEQContent, CDialog)
	//{{AFX_MSG_MAP(CDlgEQContent)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_EQ_CONTENT, OnSelchangeTabEqContent)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_EN_KILLFOCUS(IDC_EDT_LCF_GAIN_L, OnKillfocusEdtLcfGainL)
	ON_EN_KILLFOCUS(IDC_EDT_LCF_GAIN_R, OnKillfocusEdtLcfGainR)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, OnSelchangeComboFilterType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEQContent message handlers

BOOL CDlgEQContent::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Init();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEQContent::OnSelchangeTabEqContent(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	*pResult = 0;
}

void CDlgEQContent::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	for( int i=0;i<m_arrDlgEQMode.GetSize();i++ )
	{
		CDlgEQMode* pPage = m_arrDlgEQMode[i];
		pPage->DestroyWindow();
		delete pPage;
	}
}

void CDlgEQContent::DoTab(int nTab)
{
	BOOL bTab[TOTAL_EQ_MODE_NUM];
	for(int i = 0; i < TOTAL_EQ_MODE_NUM; i++)
	{
		bTab[i] = FALSE;
		if(i == nTab)
		{
			bTab[i] = TRUE;
		}
		SetDlgState(m_arrDlgEQMode[i], bTab[i]);
	}
}

void CDlgEQContent::SetDlgState(CWnd *pWnd, BOOL bShow)
{
	pWnd->EnableWindow(bShow);
	if (bShow)
	{
		pWnd->ShowWindow(SW_SHOW);
		pWnd->CenterWindow();
	} 
	else  pWnd->ShowWindow(SW_HIDE);
}

int CDlgEQContent::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDlgEQContent::Init()
{
	char szMode[64] = {0};
	for(int i = 0; i < TOTAL_EQ_MODE_NUM; i++)
	{
		CDlgEQMode *pDlg = new CDlgEQMode;
		pDlg->Create(IDD_DIALOG_EQ_MODE, &m_tabCtrl);
		if(pDlg != NULL)
		{
			m_arrDlgEQMode.Add(pDlg);
		}
		sprintf(szMode, "eq mode%d", i+1);
#ifdef _UNICODE
		TCHAR szTmp[128] = {0};
		MultiByteToWideChar(CP_ACP,0,szMode,-1,szTmp,128);
		m_tabCtrl.AddTab(m_arrDlgEQMode[i], szTmp);
#else
		m_tabCtrl.AddTab(m_arrDlgEQMode[i], (LPTSTR)szMode);
#endif
		
	}
}

void CDlgEQContent::SetPara(AUDIO_EQ_STRUCT_T EQData,BUTTERWORTH_LCF_T* lp_lcf_data )
{
	m_nEQSwitch = (EQData.eq_mode_control & 0x0800) >> 11;

	m_nSoundEqVer = GET_SOUND_EQ_VER( EQData.eq_mode_control );
	if( sound_eq_ver_4 == m_nSoundEqVer )
	{
		m_nCBFilterType = (EQData.eq_mode_control & 0x700) >> 8;
		int nLimit = EQData.eq_mode_control& 0x00FF;
		
		if(nLimit == 0)
		{
			nLimit = 1;
		}
		m_dEQLimit = (double)(20 * (double)log10((double)nLimit/(double)127));
		m_dEQLimit = (double)((short)floorf(m_dEQLimit * 10) / 10.0);
	}
	else
	{
		m_nCBFilterType = sound_eq_mode_f1f1;
		m_dEQLimit = EQData.eq_mode_control& 0x00FF;
		
		m_dEQLimit = m_dEQLimit < 0 ? 0 : m_dEQLimit;
		m_dEQLimit = m_dEQLimit > 127 ? 127 : m_dEQLimit;
	}

	if( m_dEQLimit > MAX_EQ_LIMIT )
	{
		m_dEQLimit = MAX_EQ_LIMIT;
	}
	if( m_dEQLimit < MIN_EQ_LIMIT )
	{
		m_dEQLimit = MIN_EQ_LIMIT;
	}

	m_dlimitBak = m_dEQLimit;
	// All modes use the same gain value
	int nValue = lp_lcf_data[0].lcf_gain_l;
	if(nValue == 0)
	{
		nValue = 1;
	}
	m_fLcfGainL = 20 * log10( nValue / 16384.0 );

	nValue = lp_lcf_data[0].lcf_gain_r;
	if(nValue == 0)
	{
		nValue = 1;
	}
	m_fLcfGainR = 20 * log10( nValue / 16384.0 );

	// Only remain three digit in right of the point
	m_fLcfGainL = int( m_fLcfGainL * 1000 ) / 1000.0;
	m_fLcfGainR = int( m_fLcfGainR * 1000 ) / 1000.0;
	
	if( m_fLcfGainL < MIN_LCF_GAIN_LR )
	{
		m_fLcfGainL = MIN_LCF_GAIN_LR;
	}
	if( m_fLcfGainL > MAX_LCF_GAIN_LR )
	{
		m_fLcfGainL = MAX_LCF_GAIN_LR;
	}
	if( m_fLcfGainR < MIN_LCF_GAIN_LR )
	{
		m_fLcfGainR = MIN_LCF_GAIN_LR;
	}
	if( m_fLcfGainR > MAX_LCF_GAIN_LR )
	{
		m_fLcfGainR = MAX_LCF_GAIN_LR;
	}

	for(int i = 0; i < TOTAL_EQ_MODE_NUM; i++)
	{
		m_arrDlgEQMode[i]->SetPara(EQData.eq_modes[i], EQData.eq_mode_control,lp_lcf_data[i] );
	}

	UpdateData(FALSE);

	RefreshControl();

	OnSelchangeComboFilterType();
}

BOOL CDlgEQContent::GetPara(AUDIO_EQ_STRUCT_T& EQData,BUTTERWORTH_LCF_T* lp_lcf_data )
{
	if( !UpdateData() )
	{
		return FALSE;
	}

	if( sound_eq_ver_3 == m_nSoundEqVer )
	{
		EQData.eq_mode_control = (EQData.eq_mode_control & 0xff00) | (short)m_dEQLimit;
	}
	else
	{
		if(m_dEQLimit != m_dlimitBak)
		{
			int nValue = ceilf(((double)pow(10, m_dEQLimit/20.0)) * 127.0);
			EQData.eq_mode_control = (EQData.eq_mode_control & 0xff00) | nValue;
		}
	}

	if( sound_eq_ver_4 == m_nSoundEqVer )
	{
		// All the mode use the same gain value
		lp_lcf_data[0].lcf_gain_l = 16384 * pow( 10,m_fLcfGainL / 20.0 );
		lp_lcf_data[0].lcf_gain_r = 16384 * pow( 10,m_fLcfGainR / 20.0 );
		
		EQData.eq_mode_control = (EQData.eq_mode_control & 0xf8ff) | (m_nCBFilterType << 8);
	}

	EQData.eq_mode_control = (EQData.eq_mode_control & 0xf7ff) | (m_nEQSwitch << 11);
	for(int i = 0; i < TOTAL_EQ_MODE_NUM; i++)
	{
		if( !m_arrDlgEQMode[i]->GetPara(EQData.eq_modes[i], EQData.eq_mode_control,lp_lcf_data[i] ) )
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CDlgEQContent::PreTranslateMessage(MSG* pMsg) 
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

void CDlgEQContent::OnKillfocusEdtLcfGainL() 
{
	if( UpdateData() )
	{
		for( int i=0;i<m_arrDlgEQMode.GetSize();i++ )
		{
			m_arrDlgEQMode[i]->UpdateGain( m_fLcfGainL,m_fLcfGainR );
		}
	}
}

void CDlgEQContent::OnKillfocusEdtLcfGainR() 
{
	if( UpdateData() )
	{
		for( int i=0;i<m_arrDlgEQMode.GetSize();i++ )
		{
			m_arrDlgEQMode[i]->UpdateGain( m_fLcfGainL,m_fLcfGainR );
		}
	}	
}

void CDlgEQContent::OnSelchangeComboFilterType() 
{
	UpdateData();
	
	for( int i=0;i<m_arrDlgEQMode.GetSize();i++ )
	{
		m_arrDlgEQMode[i]->ChangeFilterType( m_nCBFilterType );
	}
	
	RefreshControl();
}

void CDlgEQContent::RefreshControl()
{
	if( sound_eq_ver_3 == m_nSoundEqVer )
	{
		GetDlgItem( IDC_COMBO_FILTER_TYPE )->EnableWindow( FALSE );
	}

	if( sound_eq_mode_f1f1 == m_nCBFilterType )
	{
		GetDlgItem( IDC_EDT_LCF_GAIN_L )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDT_LCF_GAIN_R )->EnableWindow( FALSE );
	}
	else
	{
		GetDlgItem( IDC_EDT_LCF_GAIN_L )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDT_LCF_GAIN_R )->EnableWindow( TRUE );
	}
}
