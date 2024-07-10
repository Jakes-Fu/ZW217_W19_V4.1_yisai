#if !defined(AFX_DIALUPDLG_H__249F142B_8D77_4C6F_A5E4_289BCB530586__INCLUDED_)
#define AFX_DIALUPDLG_H__249F142B_8D77_4C6F_A5E4_289BCB530586__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialUpDlg.h : header file
//

#include "ModemInfoClass.h"

#include "IEtion.h"

#include "IPort.h"

#include <ras.h>

#include "CoolDlg.h"
#include "effect\CoolButton.h"

#define WM_NOTICE_SET_TIMER			(WM_USER + 209)
#define EVENT_CHECK_CONNECTED   914
/////////////////////////////////////////////////////////////////////////////
// CDialUpDlg dialog

class CDialUpDlg : public CDialog
{
// Construction
public:
	CDialUpDlg(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CDialUpDlg)

public:
	
	void Release();
	void GetErrorString(DWORD dwError);
	void SetStateText(UINT uID);

	void SetModemName(CString strModemName);
	void SetConnSpeed(DWORD dwSpd);
	void ProcessDialEvent(RASCONNSTATE rasconnstate);

private:
	BOOL InitConnect(LPCTSTR lpszModemName);
	BOOL Dial_up();
	void EnumDevices();
	BOOL CheckingConnected();
	


private:

	DWORD m_dwEntryInfoSize;
    DWORD m_dwDeviceInfoSize;
    LPRASENTRY m_lpRasEntry;
    LPBYTE m_lpDeviceInfo; 
	CString m_strModemName;
	DWORD m_dwSpd;
	DWORD m_dwDialNum;
	DWORD m_dwActualTime;
	UINT  m_nDialTime;
	BOOL  m_bKillTimer;

	CBrush m_br;
	
	RASCONNSTATE m_lastState;
	BOOL m_bExistCnnt;


// Dialog Data
	//{{AFX_DATA(CDialUpDlg)
	enum { IDD = IDD_DLG_DIALUP };
	CButton m_btnCsl;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialUpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialUpDlg)
	virtual void OnCancel();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnDialBegain(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnRasEvent(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALUPDLG_H__249F142B_8D77_4C6F_A5E4_289BCB530586__INCLUDED_)
