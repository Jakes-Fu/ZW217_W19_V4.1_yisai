// DialUpThread.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DialUpThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialUpThread

IMPLEMENT_DYNCREATE(CDialUpThread, CWinThread)

CDialUpThread::CDialUpThread()
{
	m_pDlgDialUp = NULL;
	m_dwSpd = 0;
}

CDialUpThread::~CDialUpThread()
{
	m_pDlgDialUp = NULL;
}

BOOL CDialUpThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	m_pDlgDialUp = new CDialUpDlg;
	if( m_pDlgDialUp == NULL )
	{
		return FALSE;
	}
	m_pDlgDialUp->SetModemName(m_strModemName);
	m_pDlgDialUp->SetConnSpeed(m_dwSpd);
	
	m_pDlgDialUp->Create(IDD_DLG_DIALUP);

	m_pDlgDialUp->ShowWindow(SW_SHOW);

	return TRUE;
}

int CDialUpThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	if( m_pDlgDialUp != NULL )
	{
		if(::IsWindow(m_pDlgDialUp->m_hWnd))
		{
			//m_pDlgDialUp->Release();
			::SendMessage(m_pDlgDialUp->m_hWnd, WM_CLOSE, NULL, NULL);
		}
		delete m_pDlgDialUp;
		m_pDlgDialUp = NULL;
	}
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDialUpThread, CWinThread)
	//{{AFX_MSG_MAP(CDialUpThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialUpThread message handlers
void CDialUpThread::SetModemName(CString strModemName)
{
	m_strModemName = strModemName;
}

void CDialUpThread::SetConnSpeed(DWORD dwSpd)
{
	m_dwSpd = dwSpd;
}