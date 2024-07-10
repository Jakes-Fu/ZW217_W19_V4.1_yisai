#if !defined(AFX_TAB2DLG_H__0A2A8144_FAC9_4C74_ADAE_D6E2D45DE1B0__INCLUDED_)
#define AFX_TAB2DLG_H__0A2A8144_FAC9_4C74_ADAE_D6E2D45DE1B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tab2Dlg.h : header file
//

#include "LoadDll.h"	// Added by ClassView
#define RECMESSAGE WM_USER+33
#define RECEIVE_PORT_DEFAULT 2000
#define BAUD_RATE_DEFAULT    10
/////////////////////////////////////////////////////////////////////////////
// CTab2Dlg dialog

class CTab2Dlg : public CDialog
{
// Construction
public:
	CTab2Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTab2Dlg)
	enum { IDD = IDD_RECEIVE_DIALOG };
	CListBox	m_mylistre;
	UINT	m_uiERport;
	UINT	m_uiEbaudrate;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTab2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTab2Dlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnStart();
	afx_msg void OnReceivestop();
	afx_msg void OnReceiveclear();
	afx_msg void OnSendDump();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnReceiveBuffer(WPARAM wParam, LPARAM lParam);

private:
	CString m_strPathName;
	char    m_rzFileDir[MAX_PATH];
private:
	BOOL   m_bFlagOpen; //TRUE as Open ,FLASE as Close.
	DWORD  m_dwSize;
	LPBYTE m_pReBuffer;
	FILE * m_pFile;
	BOOL   m_bFlagRe; //TRUE as Start ,FLASE as Pause.	
public:
    CLoadDll m_dllObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB2DLG_H__0A2A8144_FAC9_4C74_ADAE_D6E2D45DE1B0__INCLUDED_)
