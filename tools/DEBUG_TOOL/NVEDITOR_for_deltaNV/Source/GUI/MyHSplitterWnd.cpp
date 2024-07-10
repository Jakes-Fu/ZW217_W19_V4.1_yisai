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

void CMyHSplitterWnd::HideRightCol() //����������๤��������
{
    CWnd* pPaneHide = GetPane(0, 1);
    pPaneHide->ShowWindow(SW_HIDE);
	//�����ش�����ԴID���䵽��ǰ�е������֮��
    pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + m_nCols); 
    CWnd* pPane = GetPane(0, 0);
    pPane->SetDlgCtrlID(IdFromRowCol(0, 0)); //����һ�е���Դ��ǰ
    m_nCols--;
    RecalcLayout();
}

void CMyHSplitterWnd::ShowRightCol()
{
    m_nCols++;
	//����ID�����صĴ����ҳ���
    CWnd* pPanShow = GetDlgItem(AFX_IDW_PANE_FIRST + m_nCols);
    pPanShow->ShowWindow(SW_SHOWNA);
    CWnd* pPane = GetPane(0, 0);
    pPane->SetDlgCtrlID(IdFromRowCol(0, 0));
    pPanShow->SetDlgCtrlID(IdFromRowCol(0, 1));
    RecalcLayout();
}