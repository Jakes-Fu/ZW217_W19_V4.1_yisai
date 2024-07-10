#if !defined(AFX_SETTINGSPAGE_H__2FD3BB8B_473E_40FD_B3C5_41E2E853195E__INCLUDED_)
#define AFX_SETTINGSPAGE_H__2FD3BB8B_473E_40FD_B3C5_41E2E853195E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingsPage dialog

class CSettingsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSettingsPage)

// Construction
public:
	CSettingsPage();
	~CSettingsPage();

// Dialog Data
	//{{AFX_DATA(CSettingsPage)
	enum { IDD = IDD_SETTINGS };
	CButton	m_btnHangup;
	CButton	m_btnMakeCall;
	CEdit	m_edtPhoneNumber;
	CButton	m_btnSave;
	CButton	m_btnLoad;
	CButton	m_btnExport;
	CButton m_btnImport;
	CButton	m_btnAudio;
	CButton m_btnMusic;
	CButton m_btnMix;
	CString	m_strCSIPAddr;
	UINT	m_nCSPort;
	CString	m_strPhoneNumber;
	UINT	m_nBaudrate;
	int	m_nCBControlIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSettingsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSettingsPage)
	afx_msg void OnButtonAudio();
	afx_msg void OnConnect();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonExport();
	afx_msg void OnButtonMakeCall();
	afx_msg void OnButtonHangUp();
	afx_msg void OnButtonMixArmlog();
	afx_msg void OnButtonImport();
	afx_msg void OnConnectUart();
	afx_msg void OnButtonMusic();
	afx_msg void OnButtonExportCallParameter();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void SetConnectButtonTitle(BOOL bUart);
    void RefreshControls();
	void GetUartInfo();
	void ShowSocketCnnt(int nCmdShow);
	void ShowTestModeBtn(int nCmdShow);

public:
	void ClearState();

protected:
    BOOL m_bConnected;
    BOOL m_bLoad;
	BOOL m_bLoadArmAudio;
	BOOL m_bLoadENHA;
	BOOL m_bUsedMusic;
	BOOL m_bCSConnectMode;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSPAGE_H__2FD3BB8B_473E_40FD_B3C5_41E2E853195E__INCLUDED_)
