// TackleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "TackleDlg.h"

#include "TestPointsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTackleDlg dialog


CTackleDlg::CTackleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTackleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTackleDlg)
	m_strFilter = _T("");
	//}}AFX_DATA_INIT
    m_pParentWnd = pParent;
    m_lIndex = -1;
}


void CTackleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTackleDlg)
	DDX_Text(pDX, IDC_STATIC_FILTER_STRING, m_strFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTackleDlg, CDialog)
	//{{AFX_MSG_MAP(CTackleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTackleDlg message handlers

void CTackleDlg::OnOK() 
{
    CTestPointsView* pView = (CTestPointsView*)m_pParentWnd;
    pView->SetAutoScroll(FALSE);
    pView->GotoItem(m_lIndex);
	CDialog::OnOK();

    delete this;
}

void CTackleDlg::OnCancel() 
{
	CDialog::OnCancel();

    delete this;
}
