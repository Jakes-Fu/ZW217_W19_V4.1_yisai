// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog


CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTotalNum = 0;
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
	// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
    //ON_MESSAGE(WM_PROGRESS,OnProgress)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers
/*void CDlgProgress::OnProgress(WPARAM wParam, LPARAM lParam )
{
	_ASSERT(wParam >= 0 && lParam != NULL);

	switch(wParam) 
	{
	case PROG_BEGIN:
		ProgBegin((int)lParam);
		break;
	case PROG_PROCEED:
		ProgProceed((int)lParam);
		break;
	case PROG_END:
		ProgEnd();
		break;
	case PROG_TITLE:
		ChangTitle((LPCTSTR)lParam);
	default:
		break;
	}
}*/

void CDlgProgress::ProgBegin(int nTotalNum)
{
	m_nTotalNum = nTotalNum;
	CProgressCtrl * pProg = (CProgressCtrl *)GetDlgItem(IDC_PROG);
	//pProg->SetPos((float)1/nTotalNum * 100);
	pProg->SetRange32(0,m_nTotalNum);

}

void CDlgProgress::ProgProceed(int nCurNum)
{
	CProgressCtrl * pProg = (CProgressCtrl *)GetDlgItem(IDC_PROG);
	//pProg->SetPos((float)nCurNum / m_nTotalNum * 100);
	pProg->SetPos(nCurNum);
}

void CDlgProgress::ProgEnd()
{
	CProgressCtrl * pProg = (CProgressCtrl *)GetDlgItem(IDC_PROG);
	pProg->SetPos(100);
}
void CDlgProgress::ChangTitle(LPCTSTR lpszTitle)
{
	CWnd *pWnd = (CWnd *)GetDlgItem(IDC_STC_TITLE);
	pWnd->SetWindowText(lpszTitle);
}
