#if !defined(AFX_CLIENTINFOSHEET_H__2C04ED00_2BE6_4223_B425_33E99F9D0AE2__INCLUDED_)
#define AFX_CLIENTINFOSHEET_H__2C04ED00_2BE6_4223_B425_33E99F9D0AE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientInfoSheet.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CClientInfoSheet dialog

class CClientInfoSheet : public CPropertyPage
{
	DECLARE_DYNCREATE(CClientInfoSheet)

// Construction
public:
	void RefreshClientInfo();
	CClientInfoSheet();
	~CClientInfoSheet();

private:

// Dialog Data
	//{{AFX_DATA(CClientInfoSheet)
	enum { IDD = IDD_CLIENT_INFO };
	CListCtrl	m_ctlClientList;
	int		m_nClientCounts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CClientInfoSheet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CClientInfoSheet)
	afx_msg void OnBtnRefresh();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTINFOSHEET_H__2C04ED00_2BE6_4223_B425_33E99F9D0AE2__INCLUDED_)
