// ClientInfoSheet.cpp : implementation file
//

#include "stdafx.h"
#include "ChannelServer.h"
#include "ClientInfoSheet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CClientInfoSheet property page

IMPLEMENT_DYNCREATE(CClientInfoSheet, CPropertyPage)

CClientInfoSheet::CClientInfoSheet() : CPropertyPage(CClientInfoSheet::IDD)
{
	//{{AFX_DATA_INIT(CClientInfoSheet)
	m_nClientCounts = 0;
	//}}AFX_DATA_INIT
}

CClientInfoSheet::~CClientInfoSheet()
{
}

void CClientInfoSheet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientInfoSheet)
	DDX_Control(pDX, IDC_CLIENT_LIST, m_ctlClientList);
	DDX_Text(pDX, IDC_CLIENT_COUNTS, m_nClientCounts);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClientInfoSheet, CPropertyPage)
	//{{AFX_MSG_MAP(CClientInfoSheet)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientInfoSheet message handlers
BOOL CClientInfoSheet::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//Insert "IP Address" and "Host Name" columns into Client list control
	m_ctlClientList.InsertColumn (0,_T("IP Address"),LVCFMT_LEFT,120);
    m_ctlClientList.InsertColumn (1,_T("Host Name"),LVCFMT_LEFT,120);
	m_ctlClientList.InsertColumn( 2,_T("Tool Name"),LVCFMT_LEFT,120 );
	m_ctlClientList.InsertColumn( 3,_T("Connect time"),LVCFMT_LEFT,120 );
    
	OnBtnRefresh();
	return TRUE; 
}

void CClientInfoSheet::OnBtnRefresh() 
{
	// Refreshing client host info
	RefreshClientInfo();
}

void CClientInfoSheet::RefreshClientInfo()
{
	//Remove all items
	m_ctlClientList.DeleteAllItems ();
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	Client_Info* lpClient,*lpInfo;
	m_nClientCounts = pFrame->m_CC.GetClientInfo( &lpClient );
	lpInfo = lpClient;
	for( int i=0;i<m_nClientCounts;i++ )
	{
		m_ctlClientList.InsertItem( i,lpInfo->szIP );
		m_ctlClientList.SetItemText( i,1,lpInfo->szHostName );
		m_ctlClientList.SetItemText( i,2,lpInfo->szToolName );
		m_ctlClientList.SetItemText( i,3,_tctime( &lpInfo->time ) );
		lpInfo++;
	}

	if( m_nClientCounts > 0 )
	{
		delete lpClient;
	}

	UpdateData( FALSE );
}