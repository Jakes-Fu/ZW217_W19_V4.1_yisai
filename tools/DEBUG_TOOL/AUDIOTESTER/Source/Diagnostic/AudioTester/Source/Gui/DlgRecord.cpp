// DlgRecord.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgRecord.h"
#include "DlgRecordDp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRecord dialog


CDlgRecord::CDlgRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecord::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRecord)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ZeroMemory(&m_VolumeData, sizeof(VOLUME_STRUCT));
	m_bDspCodecInfo = FALSE;
	m_bSmartPhone = FALSE;
}


void CDlgRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRecord)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRecord, CDialog)
	//{{AFX_MSG_MAP(CDlgRecord)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_DP_LOAD_CURRENT_MODE_FROM_FLASH, OnButtonDpLoadCurrentModeFromFlash)
	ON_COMMAND(ID_BUTTON_DP_LOAD_CURRENT_MODE_FROM_RAM, OnButtonDpLoadCurrentModeFromRam)
	ON_COMMAND(ID_BUTTON_DP_WRITE_CURRENT_MODE_TO_FLASH, OnButtonDpWriteCurrentModeToFlash)
	ON_COMMAND(ID_BUTTON_DP_WRITE_CURRENT_MODE_TO_RAM, OnButtonDpWriteCurrentModeToRam)
	ON_BN_CLICKED(IDC_BUTTON_NORMAL_RECORD, OnButtonNormalRecord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRecord message handlers

BOOL CDlgRecord::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitToolbar();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRecord::SetValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&m_eq_data, &eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRecord::FetchValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&eq_data, &m_eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRecord::InitToolbar()
{
    CRect reResultPos;
    GetDlgItem( IDC_STATIC_TOOLBAR_PLACEHOLDER_AUDIO )->GetWindowRect( &reResultPos );
    ScreenToClient( &reResultPos );
    m_toolBarDP.MoveWindow( &reResultPos );
    
    m_toolBarDP.ShowWindow( SW_SHOW );
}

int CDlgRecord::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_toolBarDP.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_toolBarDP.LoadToolBar(IDR_TOOLBAR_DP))
	{
		DWORD dwErr = GetLastError();
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	CToolBarCtrl &ctl = m_toolBarDP.GetToolBarCtrl();
	DWORD dwStyle = ctl.GetStyle();
	dwStyle |= CBRS_TOOLTIPS;
	ctl.SetStyle(dwStyle);
	return 0;
}

BOOL CDlgRecord::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
			case ID_BUTTON_DP_WRITE_CURRENT_MODE_TO_RAM:
				strTip.LoadString(IDS_WRITE_EQ_MS);
				break;
			case ID_BUTTON_DP_WRITE_CURRENT_MODE_TO_FLASH:
				strTip.LoadString(IDS_WRITE_EQ_FLASH);
				break;
			case ID_BUTTON_DP_LOAD_CURRENT_MODE_FROM_RAM:
				strTip.LoadString(IDS_LOAD_EQ_MS);
				break;
			case ID_BUTTON_DP_LOAD_CURRENT_MODE_FROM_FLASH:
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

void CDlgRecord::OnButtonDpLoadCurrentModeFromFlash() 
{
	// TODO: Add your command handler code here
	if(m_pSheet != NULL)
	{	
		m_pSheet->LoadENHAModes(TYPE_EQ, GET_ENHA_DATA_FROM_FLASH);
		SetValue(m_pSheet->m_eqData_g.lpEQMode[m_nEQIndex]);

		m_pSheet->GetVolumeParam(m_nPageIndex, GET_ARM_VOLUME_DATA_FROM_FLASH);
		memcpy(&m_VolumeData, &m_pSheet->m_VolumeData.lpVolumeMode[m_nPageIndex].volume_data, sizeof(VOLUME_STRUCT));
		SetArmValue(m_VolumeData);
	}

}

void CDlgRecord::OnButtonDpLoadCurrentModeFromRam() 
{
	// TODO: Add your command handler code here
	if(m_pSheet != NULL)
	{	
		m_pSheet->LoadENHAModes(TYPE_EQ, GET_ENHA_DATA_FROM_RAM);
		SetValue(m_pSheet->m_eqData_g.lpEQMode[m_nEQIndex]);

		m_pSheet->GetVolumeParam(m_nPageIndex, GET_ARM_VOLUME_DATA_FROM_RAM);
		memcpy(&m_VolumeData, &m_pSheet->m_VolumeData.lpVolumeMode[m_nPageIndex].volume_data, sizeof(VOLUME_STRUCT));
		SetArmValue(m_VolumeData);
	}
}

void CDlgRecord::OnButtonDpWriteCurrentModeToFlash() 
{
	// TODO: Add your command handler code here
	if(m_pSheet != NULL)
	{
		FetchValue(m_pSheet->m_eqData_g.lpEQMode[m_nEQIndex]);
		m_pSheet->WriteENHAParaToPhone(m_nEQIndex, TYPE_EQ, SET_ENHA_DATA_TO_FLASH,0);

		memcpy(&m_pSheet->m_VolumeData.lpVolumeMode[m_nPageIndex].volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
		m_pSheet->WriteVolumeParam(m_nPageIndex, SET_ARM_VOLUME_DATA_TO_FLASH);
	}
}

void CDlgRecord::OnButtonDpWriteCurrentModeToRam() 
{
	// TODO: Add your command handler code here
	if(m_pSheet != NULL)
	{
		FetchValue(m_pSheet->m_eqData_g.lpEQMode[m_nEQIndex]);
		m_pSheet->WriteENHAParaToPhone(m_nEQIndex, TYPE_EQ, SET_ENHA_DATA_TO_RAM,1); //1 is first presets index

		memcpy(&m_pSheet->m_VolumeData.lpVolumeMode[m_nPageIndex].volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
		m_pSheet->WriteVolumeParam(m_nPageIndex, SET_ARM_VOLUME_DATA_TO_RAM);
	}
}

void CDlgRecord::OnButtonNormalRecord() 
{
	// TODO: Add your control notification handler code here
	CDlgRecordDp dlg;
	dlg.SetValue(m_eq_data);
	dlg.SetArmValue(m_VolumeData);
	dlg.SetControlFlag(m_bDspCodecInfo, m_bSmartPhone);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_eq_data);
		dlg.FetchArmValue(m_VolumeData);
	}
	
}

void CDlgRecord::SetArmValue( VOLUME_STRUCT& volume_data )
{
	memcpy(&m_VolumeData, &volume_data, sizeof(VOLUME_STRUCT));
}

void CDlgRecord::FetchArmValue( VOLUME_STRUCT& volume_data )
{
	memcpy(&volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
}

void CDlgRecord::FetchValueEx(EQ_STRUCT_G_T& eq_data)
{
	for(int i = 0;  i < 45; i++)
	{
		eq_data.extend[i] = m_eq_data.extend[i];
	}	
}