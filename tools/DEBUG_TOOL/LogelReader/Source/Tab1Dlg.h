#if !defined(AFX_TAB1DLG_H__A956038D_7093_4810_9FB8_9922B00A9E1B__INCLUDED_)
#define AFX_TAB1DLG_H__A956038D_7093_4810_9FB8_9922B00A9E1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tab1Dlg.h : header file
//
#include "LoadDll.h"	// Added by ClassView
#define TIMER_EVENT       100
#define DELAY_DEFAULT     10000
#define SEND_PORT_DEFAULT 5000
#define SIZE_DEFAULT      10

/////////////////////////////////////////////////////////////////////////////
// CTab1Dlg dialog

class CTab1Dlg : public CDialog
{
// Construction
public:
	CTab1Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTab1Dlg)
	enum { IDD = IDD_SEND_DIALOG };
	CListBox	m_mylist;
	UINT	m_uiEdelay;
	UINT	m_uiEport;
	UINT	m_uiEsize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTab1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTab1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOpen();
	afx_msg void OnSend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetUp();
	afx_msg void OnStop();
	afx_msg void OnSendclear();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	
	UINT  m_uiTimeEvent;
	FILE* m_pFile;
	BOOL  m_bFlag;  //TRUE as Send ,FLASE as Pause.
	BYTE* m_pBuffer;
	CString m_strPathName;
	char    m_szFileDir[MAX_PATH];
public:
    CLoadDll m_dllSocObj;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB1DLG_H__A956038D_7093_4810_9FB8_9922B00A9E1B__INCLUDED_)
