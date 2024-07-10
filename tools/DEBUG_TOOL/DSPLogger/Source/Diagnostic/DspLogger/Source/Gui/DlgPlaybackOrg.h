#if !defined(AFX_DLGPLAYBACKORG_H__ED3C1AAD_36AB_444E_AC61_9033E72347DB__INCLUDED_)
#define AFX_DLGPLAYBACKORG_H__ED3C1AAD_36AB_444E_AC61_9033E72347DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPlaybackOrg.h : header file
//

#include "FileDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPlaybackOrg dialog

class CDlgPlaybackOrg : public CDialog
{
// Construction
public:
	CDlgPlaybackOrg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPlaybackOrg)
	enum { IDD = IDD_DLG_PLAYBACK_ORG };
	CString m_strOrgPath;
	CString m_strTPPath;
	CString m_strPrompt;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	CString m_strPlaybackFile;
	int m_nCount;
	
private:
	BOOL m_bConverting;
	HANDLE m_hThread;

public:
	void SetPrompt(LPSTR lpStr);
	void SetPlaybackData (void* pData, DWORD dwSize, DWORD dwReserve );
	void SetPlaybackDataEx (void* pData, DWORD dwSize, DWORD dwReserve=0,DWORD dwPCTime=0 );
	void CloseThread();
	void PlayBackTP();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPlaybackOrg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPlaybackOrg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnBrowseOrg();
	afx_msg void OnBtnBrowseTp();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnSetPromptTimer(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKillPromptTimer(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLAYBACKORG_H__ED3C1AAD_36AB_444E_AC61_9033E72347DB__INCLUDED_)
