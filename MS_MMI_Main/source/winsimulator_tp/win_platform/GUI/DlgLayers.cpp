// DlgLayers.cpp : implementation file
//

#include "stdafx.h"
#include "msdev.h"
#include "DlgLayers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLayers dialog


CDlgLayers::CDlgLayers(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgLayers::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLayers)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgLayers::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLayers)
	DDX_Control(pDX, IDC_DLS_TAB, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLayers, CResizableDialog)
	//{{AFX_MSG_MAP(CDlgLayers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLayers message handlers

BOOL CDlgLayers::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_dlgMainLcd.Create(IDD_DIALOG_MAIN_LCD_UI_LAYER,&m_tab);
	m_dlgMainLcd2.Create(IDD_DIALOG_MAIN_LCD_UI_LAYER,&m_tab);
	m_tab.AddTab(&m_dlgMainLcd, _T("Main LCD"));
	m_tab.AddTab(&m_dlgMainLcd2, _T("Main LCD2"));

	AddAnchor(IDC_DLS_TAB,TOP_LEFT,BOTTOM_RIGHT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
