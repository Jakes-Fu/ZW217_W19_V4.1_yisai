#if !defined(AFX_DLGSMSSTORE_H__9C55D482_8349_4B17_BA23_DFCAAA58EF77__INCLUDED_)
#define AFX_DLGSMSSTORE_H__9C55D482_8349_4B17_BA23_DFCAAA58EF77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSMSStore.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDlgSMSStore dialog

class CDlgSMSStore : public CDialog
{
// Construction
public:
	CDlgSMSStore(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSMSStore)
	enum { IDD = IDD_DLG_SMS_STORE };
	CComboBox m_boxRD;
	CComboBox m_boxWS;
	UINT m_uRDUsed;
	UINT m_uRDMax;
	UINT m_uWSUsed;
	UINT m_uWSMax;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
private:
	void SendCheckRequest();
	BOOL SendSetRequest(SMS_STORE_PLACE_E eStorePlaceRD, SMS_STORE_PLACE_E eStorePlaceWS );
	void ParseCPMS(CString strCPMS);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSMSStore)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSMSStore)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboRd();
	afx_msg void OnSelchangeComboWs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSMSSTORE_H__9C55D482_8349_4B17_BA23_DFCAAA58EF77__INCLUDED_)
