// LFPFeedBack.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "LFPFeedBack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INVALID_TICK -1
#define MAX_TICK   30
#define IDLE_TIMER 20

/////////////////////////////////////////////////////////////////////////////
// CLFPFeedBack dialog


CLFPFeedBack::CLFPFeedBack(CWnd* pParent /*=NULL*/)
	: CDialog(CLFPFeedBack::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLFPFeedBack)
	//}}AFX_DATA_INIT
    m_nRange = 0;

    m_nTick = INVALID_TICK;
	m_bInit = FALSE;
	m_bRead = TRUE;
}


void CLFPFeedBack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLFPFeedBack)
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	DDX_Control(pDX, IDCANCEL, m_ctlCancel);
	DDX_Control(pDX, IDC_PROGRESS, m_ctlProgress);
	DDX_Control(pDX, IDC_INFO, m_ctlInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLFPFeedBack, CDialog)
	//{{AFX_MSG_MAP(CLFPFeedBack)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_UPDTAE_STATE,OnMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLFPFeedBack message handlers

BOOL CLFPFeedBack::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ctlInfo.SetWindowText( m_strInfo );
    m_ctlProgress.SetRange( 0, (short)m_nRange );
    m_ctlProgress.SetPos( 0 );
    m_ctlCancel.EnableWindow( TRUE );

    m_nTick = MAX_TICK;
    //SetTimer( IDLE_TIMER,1000,NULL );

	m_listLog.InsertColumn(0,_T("LOG"), LVCFMT_LEFT, 500);

	m_bInit = TRUE;

	if(m_bRead)
	{
		SetWindowText(_T("Load frome phone"));
	}
	else
	{
		SetWindowText(_T("Save to phone"));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLFPFeedBack::UpdateFeedBack( const CString &strInfo, int nPos, BOOL bDisableCancel,LPCTSTR lpszLog)
{
    
    if ( NULL == GetSafeHwnd() || !m_bInit )
    {
        // The dialog was already destoryed.
        return;
    }
    
	if(strInfo.CompareNoCase(_T("KEEP")) != 0)
	{
		m_ctlInfo.SetWindowText( strInfo );
		m_ctlProgress.SetPos( nPos );
	}
    
	if(lpszLog != NULL)
	{
		m_listLog.InsertItem(m_listLog.GetItemCount(),lpszLog);
	}
    
    if ( bDisableCancel )
    {
        m_ctlCancel.EnableWindow( FALSE );
    }

    // Reset the timer
    m_nTick = MAX_TICK;
}

void CLFPFeedBack::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);

    if( m_nTick > 0 )
    {
        m_nTick--;
    }

    if( m_nTick == 0 )
    {
        // Read flash time out
        // end the dialog
        EndDialog( IDABORT );
    }
}

void CLFPFeedBack::OnMessage(WPARAM wParam,LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);
	if(wParam == IDOK)
	{
		EndDialog(IDOK);
	}
	else
	{
		EndDialog(IDABORT);
	}
}
