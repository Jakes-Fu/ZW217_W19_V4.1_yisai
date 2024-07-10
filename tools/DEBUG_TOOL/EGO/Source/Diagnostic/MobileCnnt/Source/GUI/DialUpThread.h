#if !defined(AFX_DIALUPTHREAD_H__4C77343B_8FC5_4E48_BA82_B18E5A6D65B8__INCLUDED_)
#define AFX_DIALUPTHREAD_H__4C77343B_8FC5_4E48_BA82_B18E5A6D65B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialUpThread.h : header file
//
#include "DialUpDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CDialUpThread thread

class CDialUpThread : public CWinThread
{
	DECLARE_DYNCREATE(CDialUpThread)
protected:
	CDialUpThread();           // protected constructor used by dynamic creation

// Attributes
public:
	void SetModemName(CString strModemName);
	void SetConnSpeed(DWORD dwSpd);

// Operations
public:

	CDialUpDlg *m_pDlgDialUp;
	CString m_strModemName;
	DWORD m_dwSpd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialUpThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDialUpThread();

	// Generated message map functions
	//{{AFX_MSG(CDialUpThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALUPTHREAD_H__4C77343B_8FC5_4E48_BA82_B18E5A6D65B8__INCLUDED_)
