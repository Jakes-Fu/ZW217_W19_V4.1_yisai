// DlgEQSet.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgEQSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EQ_PAGE_NUM 4
/////////////////////////////////////////////////////////////////////////////
// CDlgEQSet dialog


CDlgEQSet::CDlgEQSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEQSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEQSet)
		// NOTE: the ClassWizard will add member initialization here
	
	//}}AFX_DATA_INIT
	m_EQData.lpEQMode = NULL;
	m_nCurrentPage = 0;
	m_pSheet =NULL;
	m_nSoundEqVer = sound_eq_ver_3;
}


void CDlgEQSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEQSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_TAB_EQ_SET, m_tabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEQSet, CDialog)
	//{{AFX_MSG_MAP(CDlgEQSet)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_EQ_SET, OnSelchangeTabEqSet)
	ON_WM_DESTROY()
	ON_COMMAND(ID_BUTTON_EQ_WRITE_CURRENT_MODE_TO_FLASH, OnButtonEqWriteCurrentModeToFlash)
	ON_COMMAND(ID_BUTTON_EQ_WRITE_CURRENT_MODE_TO_PHONE, OnButtonEqWriteCurrentModeToPhone)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_EQ_LOAD_CURRENT_MODE_FROM_FLASH, OnButtonEqLoadCurrentModeFromFlash)
	ON_COMMAND(ID_BUTTON_EQ_LOAD_CURRENT_MODE_FROM_PHONE, OnButtonEqLoadCurrentModeFromPhone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEQSet message handlers

BOOL CDlgEQSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitToolbar();
	//m_pSheet = (CAudioTesterSheet *)AfxGetMainWnd();

	if(m_pSheet == NULL)
	{
		return TRUE;
	}
	int nCount = m_pSheet->m_EQData.eq_count;
	//m_EQData.lpEQMode = new AUDIO_EQ_STRUCT_T[nCount];

	memcpy(&m_EQData, &(m_pSheet->m_EQData), sizeof(EQ_DATA));
	if( !GetLcfData() )
	{
		return TRUE;
	}

	m_nSoundEqVer = GET_SOUND_EQ_VER( m_EQData.lpEQMode[0].eq_mode_control );
	
	for(int i = 0; i < nCount; i++)
	{
		CDlgEQContent *pDlg = new CDlgEQContent;
		pDlg->Create(IDD_DIALOG_EQ_CONTENT, &m_tabCtrl);
		if(pDlg != NULL)
		{
			m_arrDlgEQContent.Add(pDlg);
		}
#ifdef _UNICODE
		TCHAR szTmp[128] = {0};
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)m_EQData.lpEQMode[i].ucParaSetName,-1,szTmp,NAME_MAX_LEN);
		m_tabCtrl.AddTab(m_arrDlgEQContent[i], szTmp);
#else
		m_tabCtrl.AddTab(m_arrDlgEQContent[i], (LPTSTR)m_EQData.lpEQMode[i].ucParaSetName);
#endif
		m_arrDlgEQContent[i]->SetPara(m_EQData.lpEQMode[i],m_lcfData);
	}

	m_tabCtrl.SelectTab(m_nCurrentPage);
	
	for(i = 0; i < EQ_PAGE_NUM; i++)
	{
		if(i != m_nCurrentPage)
		{
			m_tabCtrl.EnableTab(i, FALSE);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEQSet::DoTab(int nTab)
{
	BOOL bTab[EQ_PAGE_NUM];
	for(int i = 0; i < EQ_PAGE_NUM; i++)
	{
		bTab[i] = FALSE;
		if(i == nTab)
		{
			bTab[i] = TRUE;
		}
		SetDlgState(m_arrDlgEQContent[i], bTab[i]);
	}
}

void CDlgEQSet::SetDlgState(CWnd *pWnd, BOOL bShow)
{
	pWnd->EnableWindow(bShow);
	ASSERT( pWnd->IsWindowEnabled() == bShow );

	if (bShow)
	{
		pWnd->ShowWindow(SW_SHOW);
		pWnd->CenterWindow();
	} 
	else  pWnd->ShowWindow(SW_HIDE);
}

void CDlgEQSet::OnSelchangeTabEqSet(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nSelect=m_tabCtrl.GetCurSel();
	if(nSelect>=0) 
	{
		DoTab(nSelect);
	}
	
	*pResult = 0;
}

void CDlgEQSet::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	for( int i=0;i<m_arrDlgEQContent.GetSize();i++ )
	{
		CDlgEQContent* pPage = m_arrDlgEQContent[i];
		pPage->DestroyWindow();
		delete pPage;
	}	
}

void CDlgEQSet::OnButtonEqWriteCurrentModeToFlash() 
{
	if(m_pSheet == NULL)
	{
		return;
	}
	
	if( !m_pSheet->FetchValue( m_pSheet->GetActiveIndex() - 1 ) )
	{
		m_pSheet->Log( _T( "Get audio data from gui failed" ),TRUE );
		return;
	}
	
	for(int i = 0; i < m_EQData.eq_count; i++)
	{
		if( m_tabCtrl.IsTabEnabled(i) )
		{
			if( !m_arrDlgEQContent[i]->GetPara(m_EQData.lpEQMode[i],m_lcfData) )
			{
				// Some data is not valid
				return;
			}
		}
	}
	memcpy(&(m_pSheet->m_EQData), &m_EQData, sizeof(EQ_DATA));
	SetLcfData();
	int nIndex = m_tabCtrl.GetCurSel();
	if( !m_pSheet->WriteEQParaToFlash(nIndex) )
	{
		return;
	}
	if( m_nSoundEqVer == sound_eq_ver_4 )
	{
		m_pSheet->WriteParaToFlash( m_pSheet->GetActiveIndex() - 1 );
	}
}

void CDlgEQSet::OnButtonEqWriteCurrentModeToPhone() 
{
	// TODO: Add your command handler code here

	if(m_pSheet == NULL)
	{
		return;
	}

	if( !m_pSheet->FetchValue( m_pSheet->GetActiveIndex() - 1 ) )
	{
		m_pSheet->Log( _T( "Get audio data from gui failed" ),TRUE );
		return;
	}

	for(int i = 0; i < m_EQData.eq_count; i++)
	{
		if( m_tabCtrl.IsTabEnabled(i) )
		{
			if( !m_arrDlgEQContent[i]->GetPara(m_EQData.lpEQMode[i],m_lcfData) )
			{
				return;
			}
		}
	}
	memcpy(&(m_pSheet->m_EQData), &m_EQData, sizeof(EQ_DATA));
	SetLcfData();
	int nIndex = m_tabCtrl.GetCurSel();
	if( !m_pSheet->WriteEQParaToPhone(nIndex) )
	{
		return;
	}

	if( m_nSoundEqVer == sound_eq_ver_4 )
	{
		m_pSheet->WriteParaToPhone( m_pSheet->GetActiveIndex() - 1 );
	}
}

void CDlgEQSet::InitToolbar()
{
    CRect reResultPos;
    GetDlgItem( IDC_STATIC_TOOLBAR_PLACEHOLDER )->GetWindowRect( &reResultPos );
    ScreenToClient( &reResultPos );
    m_toolBarEQ.MoveWindow( &reResultPos );
    
    m_toolBarEQ.ShowWindow( SW_SHOW );
}

int CDlgEQSet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_toolBarEQ.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_toolBarEQ.LoadToolBar(IDR_TOOLBAR_EQ))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }
    
	CToolBarCtrl &ctl = m_toolBarEQ.GetToolBarCtrl();	
	DWORD dwStyle = ctl.GetStyle();
	dwStyle |= CBRS_TOOLTIPS;
	ctl.SetStyle(dwStyle);
	
	return 0;
}

BOOL CDlgEQSet::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	TOOLTIPTEXT *pTip = (TOOLTIPTEXT *)lParam;  

	CString strTip;  
	
	switch(pTip->hdr.code)  
	{  	
	case TTN_NEEDTEXT: 
		{
			switch(pTip->hdr.idFrom)  
			{  
			case ID_BUTTON_EQ_WRITE_CURRENT_MODE_TO_PHONE:
				strTip.LoadString(IDS_WRITE_EQ_MS);
				break;
			case ID_BUTTON_EQ_WRITE_CURRENT_MODE_TO_FLASH:
				strTip.LoadString(IDS_WRITE_EQ_FLASH);
				break;
			case ID_BUTTON_EQ_LOAD_CURRENT_MODE_FROM_PHONE:
				strTip.LoadString(IDS_LOAD_EQ_MS);
				break;
			case ID_BUTTON_EQ_LOAD_CURRENT_MODE_FROM_FLASH:
				strTip.LoadString(IDS_LOAD_EQ_FLASH);
				break;
			default:
				break;
			}  

			_tcscpy(pTip->szText, strTip.GetBuffer(strTip.GetLength()));  
			break;  
		}
	default:
		break;
	}
	
	strTip.ReleaseBuffer( strTip.GetLength() ); 
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CDlgEQSet::OnOK() 
{
	if( !UpdateData() )
	{
		return;
	}

	for(int i = 0; i < m_EQData.eq_count; i++)
	{
		if( m_tabCtrl.IsTabEnabled(i) )
		{
			if( !m_arrDlgEQContent[i]->GetPara(m_EQData.lpEQMode[i],m_lcfData) )
			{
				return;
			}
		}
	}
	
	CDialog::OnOK();
}

void CDlgEQSet::OnButtonEqLoadCurrentModeFromFlash() 
{
	if(m_pSheet != NULL)
	{
		if( !m_pSheet->LoadEQModes(1) )
		{
			return;
		}

		if( m_nSoundEqVer == sound_eq_ver_4 )
		{
			if( !m_pSheet->LoadParaFromFlash( m_pSheet->GetActiveIndex() - 1,FALSE ) )
			{
				return;
			}
		}
	}
	GetEQPara();
}

void CDlgEQSet::OnButtonEqLoadCurrentModeFromPhone() 
{
	if(m_pSheet != NULL)
	{
		if( !m_pSheet->LoadEQModes() )
		{
			return;
		}
		if( m_nSoundEqVer == sound_eq_ver_4 )
		{
			if( !m_pSheet->LoadParaFromPhone( m_pSheet->GetActiveIndex() - 1,FALSE ) )
			{
				return;
			}
		}
	}
	GetEQPara();
}

void CDlgEQSet::GetEQPara()
{
	int nCount = m_pSheet->m_EQData.eq_count;

	memcpy(&m_EQData, &(m_pSheet->m_EQData), sizeof(EQ_DATA));
	GetLcfData();
	
	for(int i = 0; i < nCount; i++)
	{
		if( m_tabCtrl.IsTabEnabled(i) )
		{
			m_arrDlgEQContent[i]->SetPara(m_EQData.lpEQMode[i],m_lcfData);
		}
	}
}

BOOL CDlgEQSet::GetLcfData()
{
	// Get butterworth data
	if( eq_platform_all == m_pSheet->m_EQData.eq_platform )
	{
		for( int i=0;i<TOTAL_EQ_MODE_NUM;i++ )
		{
			m_lcfData[i].fp_l = m_EQData.lpEQMode[m_nCurrentPage].extend_array[10+i*2];
			m_lcfData[i].fp_r = m_EQData.lpEQMode[m_nCurrentPage].extend_array[11+i*2];
			m_lcfData[i].lcf_gain_l = m_EQData.lpEQMode[m_nCurrentPage].extend_array[8];
			m_lcfData[i].lcf_gain_r = m_EQData.lpEQMode[m_nCurrentPage].extend_array[9];
		}
	}
	else
	{
		int nPageIndex = m_pSheet->GetActiveVolumePageIndex();
		if( nPageIndex < 0 )
		{
			m_pSheet->Log( _T( "Can not get current audio page index!" ) );
			return FALSE;
		}
		
		for( int i=0;i<TOTAL_EQ_MODE_NUM;i++ )
		{
			m_lcfData[i].fp_l = m_pSheet->m_VolumeData.lpVolumeMode[nPageIndex].volume_data.reserve[i*2+1];
			m_lcfData[i].fp_r = m_pSheet->m_VolumeData.lpVolumeMode[nPageIndex].volume_data.reserve[i*2+2];
			m_lcfData[i].lcf_gain_l = m_pSheet->m_VolumeData.lpVolumeMode[nPageIndex].volume_data.reserve[0];
			m_lcfData[i].lcf_gain_r = m_pSheet->m_VolumeData.lpVolumeMode[nPageIndex].volume_data.reserve[17];
		}
	}
	return TRUE;
}

BOOL CDlgEQSet::SetLcfData()
{
	if( eq_platform_all == m_pSheet->m_EQData.eq_platform )
	{
		for( int i=0;i<TOTAL_EQ_MODE_NUM;i++ )
		{
			m_EQData.lpEQMode[m_nCurrentPage].extend_array[10+i*2] = m_lcfData[i].fp_l;
			m_EQData.lpEQMode[m_nCurrentPage].extend_array[11+i*2] = m_lcfData[i].fp_r;
		}
		// All mode use the same gain value
		m_EQData.lpEQMode[m_nCurrentPage].extend_array[8] = m_lcfData[0].lcf_gain_l;
		m_EQData.lpEQMode[m_nCurrentPage].extend_array[9] = m_lcfData[0].lcf_gain_r;
	}
	else
	{
		int nPageIndex = m_pSheet->GetActiveVolumePageIndex();
		if( nPageIndex < 0 )
		{
			m_pSheet->Log( _T( "Can not get current audio page index!" ) );
			return FALSE;
		}
		
		for( int i=0;i<TOTAL_EQ_MODE_NUM;i++ )
		{
			m_pSheet->m_VolumeData.lpVolumeMode[nPageIndex].volume_data.reserve[i*2+1] = m_lcfData[i].fp_l;
			m_pSheet->m_VolumeData.lpVolumeMode[nPageIndex].volume_data.reserve[i*2+2] = m_lcfData[i].fp_r;
		}
		// All mode use the same gain value
		m_pSheet->m_VolumeData.lpVolumeMode[nPageIndex].volume_data.reserve[0] = m_lcfData[0].lcf_gain_l;
		m_pSheet->m_VolumeData.lpVolumeMode[nPageIndex].volume_data.reserve[17] = m_lcfData[0].lcf_gain_r;
	}	
	return TRUE;
}
