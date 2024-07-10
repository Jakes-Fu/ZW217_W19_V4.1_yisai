// SettingsPage.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "SettingsPage.h"

#include "AudioTesterSheet.h"
#include "EnumPorts.h"

#include <dbt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsPage property page

IMPLEMENT_DYNCREATE(CSettingsPage, CPropertyPage)

CSettingsPage::CSettingsPage() : CPropertyPage(CSettingsPage::IDD)
{
	//{{AFX_DATA_INIT(CSettingsPage)
	m_strCSIPAddr = _T("127.0.0.1");
	m_nCSPort = 36666;
	m_strPhoneNumber = _T("");
	m_nBaudrate = 460800;
	m_nCBControlIndex = 1;
	//}}AFX_DATA_INIT

    m_bConnected = FALSE;
    m_bLoad = FALSE;
	m_bLoadArmAudio = FALSE;
	m_bLoadENHA = FALSE;
	m_bUsedMusic = FALSE;
	m_bCSConnectMode = FALSE;
}

CSettingsPage::~CSettingsPage()
{
}

void CSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsPage)
	DDX_Control(pDX, IDC_BUTTON_HANG_UP, m_btnHangup);
	DDX_Control(pDX, IDC_BUTTON_MAKE_CALL, m_btnMakeCall);
	DDX_Control(pDX, IDC_EDIT_PHONE_NUMBER, m_edtPhoneNumber);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_btnLoad);
	DDX_Control(pDX, IDC_BUTTON_EXPORT, m_btnExport);
	DDX_Control(pDX, IDC_BUTTON_IMPORT, m_btnImport);
	DDX_Control(pDX, IDC_BUTTON_AUDIO, m_btnAudio);
	DDX_Control(pDX, IDC_BUTTON_MUSIC, m_btnMusic);
	DDX_Control(pDX, IDC_BUTTON_MIX_ARMLOG, m_btnMix);
	DDX_Text(pDX, IDC_CS_IPADDR, m_strCSIPAddr);
	DDX_Text(pDX, IDC_CS_PORT, m_nCSPort);
	DDV_MinMaxUInt(pDX, m_nCSPort, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_PHONE_NUMBER, m_strPhoneNumber);
	DDX_Text(pDX, IDC_CS_PORT_BAUDRATE, m_nBaudrate);
	DDX_CBIndex(pDX, IDC_COMBO_CONTROL_BY, m_nCBControlIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSettingsPage)
	ON_BN_CLICKED(IDC_BUTTON_AUDIO, OnButtonAudio)
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_CALL, OnButtonMakeCall)
	ON_BN_CLICKED(IDC_BUTTON_HANG_UP, OnButtonHangUp)
	ON_BN_CLICKED(IDC_BUTTON_MIX_ARMLOG, OnButtonMixArmlog)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_CONNECT_UART, OnConnectUart)
	ON_BN_CLICKED(IDC_BUTTON_MUSIC, OnButtonMusic)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_CALL_PARAMETER, OnButtonExportCallParameter)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsPage message handlers

void CSettingsPage::OnButtonAudio() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
	pSheet->OpenMixMode();

	if( !pSheet->ConfirmLoad() )
    {
        return;
    }

	if(pSheet->m_bLoadVolume && pSheet->m_bLoadENHA)
	{
		pSheet->ClearAllMusicModes();
	}

	pSheet->LoadEQModes();
	m_bLoadENHA = pSheet->IsSurpportEnha();

    m_bLoad = pSheet->LoadAudioModes();
	
    RefreshControls();
	m_bUsedMusic = FALSE;

	//teana hu 2012.02.03
	if(pSheet->m_bDspCodec)
	{
		m_nCBControlIndex = 0;
	}
	else
	{
		m_nCBControlIndex = 1;
	}
	//
}

void CSettingsPage::OnConnect() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    if( m_bConnected )
    {
        pSheet->Disconnect();
        m_bConnected = FALSE;
    }
    else
    {
        UpdateData( TRUE );
        m_bConnected = pSheet->Connect( m_strCSIPAddr,m_nCSPort );
		m_bCSConnectMode = TRUE;
    }
    RefreshControls();
    SetConnectButtonTitle(FALSE);
}

BOOL CSettingsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	GetDlgItem( IDC_BUTTON_AUDIO )->EnableWindow( m_bConnected );
	GetDlgItem( IDC_BUTTON_MUSIC )->EnableWindow( m_bConnected );
	GetDlgItem(IDC_BUTTON_MIX_ARMLOG)->EnableWindow(m_bConnected);
    SetConnectButtonTitle(TRUE);
	SetConnectButtonTitle(FALSE);
	GetUartInfo();

    RefreshControls();

	//teana hu 2011.04.02
	TCHAR szCfgPathName[MAX_PATH];
	::GetModuleFileName( NULL, szCfgPathName, _MAX_PATH );
	LPTSTR pExt = _tcsrchr(szCfgPathName, _T('.'));
	_tcscpy(pExt + 1, _T("ini"));
	int nShow = GetPrivateProfileInt( _T("Display"), _T("ShowCnntToCS"), SW_HIDE, szCfgPathName );
	ShowSocketCnnt(nShow);
	//

	//teana hu 2011.06.07
	int nShowTestMode = GetPrivateProfileInt( _T("Display"), _T("ShowTestModeBtn"), SW_HIDE, szCfgPathName );
	ShowTestModeBtn(nShowTestMode);
	//

	int nShowExport = GetPrivateProfileInt( _T("Display"),_T("ExportCallData"),SW_HIDE,szCfgPathName );
	if( SW_HIDE == nShowExport )
	{
		GetDlgItem( IDC_BUTTON_EXPORT_CALL_PARAMETER )->ShowWindow( nShowExport );
	}
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingsPage::SetConnectButtonTitle(BOOL bUart)
{
    CString strTitle;
    if( m_bConnected )
    {
        strTitle.LoadString( IDS_DISCONNECT );
    }
    else
    {
        strTitle.LoadString( IDS_CONNECT );
    }

	if( bUart )
	{
		GetDlgItem( IDC_CONNECT_UART )->SetWindowText( strTitle );
		GetDlgItem( IDC_CONNECT )->EnableWindow( !m_bConnected );
	}
	else
	{
		GetDlgItem( IDC_CONNECT )->SetWindowText( strTitle );
		GetDlgItem( IDC_CONNECT_UART )->EnableWindow( !m_bConnected );
	}
}

void CSettingsPage::OnButtonSave() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    pSheet->Save();
}

void CSettingsPage::OnButtonLoad() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    m_bLoad = pSheet->Load();
    RefreshControls();
}

void CSettingsPage::OnButtonExport() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
	if(!m_bUsedMusic)
	{
		pSheet->Export( FALSE );  // Export all data
	}
	else
	{
		pSheet->ExportMusic();
	}
}

void CSettingsPage::RefreshControls()
{
	//teana hu 2011.04.01
	GetDlgItem( IDC_BUTTON_MUSIC )->EnableWindow(m_bLoadENHA);
	//
    m_btnAudio.EnableWindow( m_bConnected );
	m_btnMusic.EnableWindow(m_bConnected);
    m_btnSave.EnableWindow( m_bLoad | m_bLoadArmAudio);
	//test
	//m_bUsedMusic = TRUE;
    m_btnExport.EnableWindow( m_bLoad | m_bLoadArmAudio);
	//test
	GetDlgItem( IDC_BUTTON_EXPORT_CALL_PARAMETER )->EnableWindow( m_bLoad | m_bLoadArmAudio );

    m_btnMakeCall.EnableWindow( m_bConnected );
    m_edtPhoneNumber.EnableWindow( m_bConnected );
    m_btnHangup.EnableWindow( m_bConnected );

	//teana hu 2009.08.24
	m_btnMix.EnableWindow(m_bConnected);
}

void CSettingsPage::OnButtonMakeCall() 
{
    UpdateData( TRUE );
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    pSheet->MakeCall( m_strPhoneNumber );    
}

void CSettingsPage::OnButtonHangUp() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    pSheet->Hangup();    
}

void CSettingsPage::OnButtonMixArmlog() 
{
	// TODO: Add your control notification handler code here
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    pSheet->SetMixModes();
}

void CSettingsPage::OnButtonImport() 
{
	// TODO: Add your control notification handler code here
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    m_bLoad = pSheet->Import();
}

void CSettingsPage::GetUartInfo()
{
	CEnumPort  ep;
	EnumPortInfo* pPortInfo;
	UINT dwCount=0;

	CComboBox* pCombo = (CComboBox*)GetDlgItem( IDC_COMBO_UART_PORT );
	if( pCombo != NULL )
	{
		pCombo->ResetContent();
		
		ep.EnumPort( &pPortInfo,&dwCount );
		for( DWORD i=0;i<dwCount;i++ )
		{
			pCombo->AddString( pPortInfo[i].szFriendlyName );
			pCombo->SetItemData( i,pPortInfo[i].nPortNum );
		}
		
		pCombo->SetCurSel( 0 );
	}
}

void CSettingsPage::OnConnectUart() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
    if( m_bConnected )
    {
        pSheet->Disconnect();
        m_bConnected = FALSE;
    }
    else
    {
		UpdateData();
		CComboBox* pCombo = (CComboBox*)GetDlgItem( IDC_COMBO_UART_PORT );
		DWORD dwPort = 0;
		if( pCombo != NULL )
		{
			int nIndex = pCombo->GetCurSel();
			if( nIndex >= 0 )
			{
				dwPort = pCombo->GetItemData( nIndex );
			}
		}

        m_bConnected = pSheet->ConnectUart( dwPort,m_nBaudrate );
		m_bCSConnectMode = FALSE;
    }

    RefreshControls();
    SetConnectButtonTitle( TRUE );
}

LRESULT CSettingsPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    if( message == WM_DEVICECHANGE )
    {        
        switch(wParam)
        {
		case DBT_DEVICEARRIVAL:
			// Refresh uart info
			GetUartInfo();
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			if(!m_bCSConnectMode)
			{
				GetUartInfo();
				if(m_bConnected)
				{
					OnConnectUart();
					CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
					if(pSheet != NULL)
					{
						if(m_bUsedMusic)
						{
							pSheet->ClearAllMusicModes();
						}
						else
						{
							pSheet->ClearAllModes();
						}
					}
					pSheet->m_bLoadENHA = FALSE;
					pSheet->m_bLoadVolume = FALSE;
					pSheet->m_bLoadEQ = FALSE;
					pSheet->m_bLoadDspCodec = FALSE;
					pSheet->m_bLoadLVVE = FALSE;
					m_bLoad = FALSE;
					m_bLoadArmAudio = FALSE;
					m_bLoadENHA = FALSE;
					m_bUsedMusic = FALSE;
				}
			}
			break;
        default:
            break;
        }
    }	
	
	return CPropertyPage::WindowProc(message, wParam, lParam);
}

void CSettingsPage::OnButtonMusic() 
{
	// TODO: Add your control notification handler code here

	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
	if(!pSheet->m_bLoadENHA)
	{
		if(!pSheet->IsSurpportEnha(TYPE_TUN))
		{
			return;
		}
		pSheet->m_bLoadENHA = FALSE;
	}
	pSheet->OpenMixMode();

	if(m_bLoad)
	{
		pSheet->ClearAllModes();
	}
	
    m_bLoadArmAudio = pSheet->LoadArmAudioModes();
	if(m_bLoadArmAudio)
	{	
		if(!pSheet->LoadENHAModes(TYPE_EQ, GET_ENHA_DATA_FROM_RAM) || !pSheet->LoadENHAModes(TYPE_TUN, GET_ENHA_DATA_FROM_RAM))
		{
			pSheet->ClearAllMusicModes();
		}
		else
		{
			RefreshControls();
			m_bUsedMusic = TRUE;
		}
	}
}

void CSettingsPage::ShowSocketCnnt(int nCmdShow)
{
	GetDlgItem(IDC_STATIC_CS_CONN)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_IP)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_PORT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_CS_IPADDR)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_CS_PORT)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_CONNECT)->ShowWindow(nCmdShow);
}

void CSettingsPage::ShowTestModeBtn(int nCmdShow)
{
	GetDlgItem(IDC_BUTTON_MIX_ARMLOG)->ShowWindow(nCmdShow);
}

void CSettingsPage::ClearState()
{
	if(!m_bCSConnectMode)
	{
		GetUartInfo();
		if(m_bConnected)
		{
			OnConnectUart();
			CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
			/*
			if(pSheet != NULL)
			{
			if(m_bUsedMusic)
			{
			pSheet->ClearAllMusicModes();
			}
			else
			{
			pSheet->ClearAllModes();
			}
			}
			*/
			pSheet->m_bLoadENHA = FALSE;
			pSheet->m_bLoadVolume = FALSE;
			pSheet->m_bLoadEQ = FALSE;
			pSheet->m_bLoadDspCodec = FALSE;
			pSheet->m_bLoadLVVE = FALSE;
			m_bLoad = FALSE;
			m_bLoadArmAudio = FALSE;
			m_bLoadENHA = FALSE;
			m_bUsedMusic = FALSE;
		}
	}
}

void CSettingsPage::OnButtonExportCallParameter() 
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)GetParent();
	if(!m_bUsedMusic)
	{
		pSheet->Export( TRUE );  // Export call data only
	}	
}

UINT CSettingsPage::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CPropertyPage::OnNcHitTest(point);
	return HTTRANSPARENT;
}
