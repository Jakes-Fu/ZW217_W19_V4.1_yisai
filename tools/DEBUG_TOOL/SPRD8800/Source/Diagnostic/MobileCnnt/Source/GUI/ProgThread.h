#if !defined(AFX_PROGTHREAD_H__42056B82_6BA6_4D02_B031_09C702AA446A__INCLUDED_)
#define AFX_PROGTHREAD_H__42056B82_6BA6_4D02_B031_09C702AA446A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgThread.h : header file
//

#include "DlgProgress.h"


/////////////////////////////////////////////////////////////////////////////
// CProgThread thread

class CProgThread : public CWinThread
{
	DECLARE_DYNCREATE(CProgThread)
protected:
	CProgThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CProgThread();
    CDlgProgress  m_dlgProgress;

	// Generated message map functions
	//{{AFX_MSG(CProgThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
    afx_msg void OnProgress(WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGTHREAD_H__42056B82_6BA6_4D02_B031_09C702AA446A__INCLUDED_)
