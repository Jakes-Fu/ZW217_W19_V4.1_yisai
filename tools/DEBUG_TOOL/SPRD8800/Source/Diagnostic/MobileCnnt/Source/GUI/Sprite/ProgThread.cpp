// ProgThread.cpp : implementation file
//

#include "stdafx.h"
#include "ProgThread.h"
#include "MsgIDDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgThread

IMPLEMENT_DYNCREATE(CProgThread, CWinThread)

CProgThread::CProgThread()
{
}

CProgThread::~CProgThread()
{
}

BOOL CProgThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
//	m_dlgProgress.Create(IDD_DLG_PROGRESS); 
//	m_dlgProgress.CenterWindow();
//	m_dlgProgress.GetParent()->EnableWindow(FALSE);
//	m_dlgProgress.ShowWindow(SW_SHOW);	
	m_dlgProgress.DoModal();
	m_pMainWnd = &m_dlgProgress;
//	MSG msg;
//	while(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//		if(!AfxGetApp()->PumpMessage())
//		{
//			::PostQuitMessage(0);
//			return FALSE;
//		}
//		if(msg.hwnd == m_pMainWnd->m_hWnd && msg.message == WM_QUIT)
//			break;
//	}
	return TRUE;
}

int CProgThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
//	m_dlgProgress.GetParent()->EnableWindow(TRUE);
	m_dlgProgress.DestroyWindow();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProgThread, CWinThread)
	//{{AFX_MSG_MAP(CProgThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PROGRESS,OnProgress)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgThread message handlers
void CProgThread::OnProgress(WPARAM wParam, LPARAM lParam )
{	
	switch(wParam) {
	case PROG_BEGIN:
		m_dlgProgress.ProgBegin((int)lParam);
		//m_dlgProgress.UpdateWindow();
		break;
	case PROG_PROCEED:
		m_dlgProgress.ProgProceed((int)lParam);
		//m_dlgProgress.UpdateWindow();
		break;
	case PROG_END:
		m_dlgProgress.ProgEnd();
		//m_dlgProgress.UpdateWindow();
		break;
	case PROG_TITLE:
		m_dlgProgress.ChangTitle((LPCTSTR)lParam);
		//m_dlgProgress.UpdateWindow();
		break;
	default:
		break;
	}
}

BOOL CProgThread::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE ||pMsg->wParam == VK_RETURN  )//ESC or Enter
		{
			return TRUE;
		}
	}
	if(pMsg->message == WM_DEVICECHANGE)
	{
		return ExitInstance();
	}
	return CWinThread::PreTranslateMessage(pMsg);
}
