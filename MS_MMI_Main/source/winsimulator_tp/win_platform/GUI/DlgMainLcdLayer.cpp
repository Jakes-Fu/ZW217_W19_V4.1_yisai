// DlgMainLcdLayer.cpp : implementation file
//

#include "stdafx.h"
#include "msdev.h"
#include "DlgMainLcdLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMainLcdLayer dialog


CDlgMainLcdLayer::CDlgMainLcdLayer(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgMainLcdLayer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMainLcdLayer)
	m_bDrawBorer = TRUE;
	m_bDrawRect = TRUE;
	m_bDrawInfo = TRUE;
	//}}AFX_DATA_INIT
	m_bShow = FALSE;
}


void CDlgMainLcdLayer::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMainLcdLayer)
	DDX_Control(pDX, IDC_DML_STC_DISP, m_stcDisp);
	DDX_Check(pDX, IDC_DLI_BORDER, m_bDrawBorer);
	DDX_Check(pDX, IDC_DLI_RECT, m_bDrawRect);
	DDX_Check(pDX, IDC_DLI_INFO, m_bDrawInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMainLcdLayer, CResizableDialog)
	//{{AFX_MSG_MAP(CDlgMainLcdLayer)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_DLI_BORDER, OnDrawBorder)
	ON_BN_CLICKED(IDC_DLI_RECT, OnDrawRect)
	ON_BN_CLICKED(IDC_DLI_INFO, OnDrawInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMainLcdLayer message handlers

BOOL CDlgMainLcdLayer::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	ShowSizeGrip(TRUE);
	AddAnchor(IDC_DML_STC_DISP,TOP_LEFT,BOTTOM_RIGHT);
	AddAnchor(IDC_DLI_BORDER,TOP_LEFT,TOP_LEFT);
	AddAnchor(IDC_DLI_RECT,TOP_LEFT,TOP_LEFT);


	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMainLcdLayer::OnSize(UINT nType, int cx, int cy) 
{
	CResizableDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

BOOL CDlgMainLcdLayer::Init(DISPLAY_BLEND_LAYER_T *pLayerInfo, DWORD dwLcdHeight, DWORD dwLcdWidth)
{
	return m_stcDisp.Init(pLayerInfo,dwLcdHeight,dwLcdWidth);
}

void CDlgMainLcdLayer::OnCancel() 
{
	// TODO: Add extra cleanup here	
	//CResizableDialog::OnCancel();
	ShowWindow(SW_HIDE);
	m_bShow = FALSE;
}

void CDlgMainLcdLayer::OnOK() 
{
	// TODO: Add extra cleanup here	
	ShowWindow(SW_HIDE);
	m_bShow = FALSE;
}

void CDlgMainLcdLayer::LayerInvalidate()
{
	m_stcDisp.LayerInvalidate();
}
void CDlgMainLcdLayer::LayerInvalidateRect(WORD left,WORD top,WORD right,WORD bottom)
{
	m_stcDisp.LayerInvalidateRect(left,top,right,bottom);
}

void CDlgMainLcdLayer::OnDrawBorder() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_stcDisp.m_bDrawBorder = m_bDrawBorer;
	m_stcDisp.Invalidate();
}

void CDlgMainLcdLayer::OnDrawRect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_stcDisp.m_bDrawRect = m_bDrawRect;
	m_stcDisp.Invalidate();
}

void CDlgMainLcdLayer::OnDrawInfo() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_stcDisp.m_bDrawLayerInfo = m_bDrawInfo;
	m_stcDisp.Invalidate();
}
