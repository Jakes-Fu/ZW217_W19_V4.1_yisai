// MyHSplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "MyHSplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyHSplitterWnd

CMyHSplitterWnd::CMyHSplitterWnd()
{
}

CMyHSplitterWnd::~CMyHSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CMyHSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CMyHSplitterWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyHSplitterWnd message handlers

void CMyHSplitterWnd::HideRightCol() //用于隐藏左侧工具栏窗口
{
    CWnd* pPaneHide = GetPane(0, 1);
    pPaneHide->ShowWindow(SW_HIDE);
	//将隐藏窗口资源ID分配到当前行的最大列之外
    pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + m_nCols); 
    CWnd* pPane = GetPane(0, 0);
    pPane->SetDlgCtrlID(IdFromRowCol(0, 0)); //将下一列的资源提前
    m_nCols--;
    RecalcLayout();
}

void CMyHSplitterWnd::ShowRightCol()
{
    m_nCols++;
	//根据ID将隐藏的窗口找出来
    CWnd* pPanShow = GetDlgItem(AFX_IDW_PANE_FIRST + m_nCols);
    pPanShow->ShowWindow(SW_SHOWNA);
    CWnd* pPane = GetPane(0, 0);
    pPane->SetDlgCtrlID(IdFromRowCol(0, 0));
    pPanShow->SetDlgCtrlID(IdFromRowCol(0, 1));
    RecalcLayout();
}