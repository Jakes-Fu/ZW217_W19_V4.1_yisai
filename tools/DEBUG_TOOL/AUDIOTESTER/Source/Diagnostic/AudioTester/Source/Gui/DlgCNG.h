#if !defined(AFX_DLGCNG_H__1D9A6730_C26C_433C_97C6_588EC2F00AF2__INCLUDED_)
#define AFX_DLGCNG_H__1D9A6730_C26C_433C_97C6_588EC2F00AF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCNG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCNG dialog

class CDlgCNG : public CDialog
{
// Construction
public:
	CDlgCNG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCNG)
	enum { IDD = IDD_DLG_CNG };
		// NOTE: the ClassWizard will add data members here
	int m_nCBIndexCNGSW;
	int m_nCBIndexGain;
	CComboBox m_boxCNGGain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCNG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetValue(DWORD dwCNGGain, DWORD dwCNGSW){m_dwCNGGain = dwCNGGain; m_dwCNGSW = dwCNGSW;}
	void FetchValue(DWORD &dwCNGGain, DWORD &dwCNGSW){dwCNGGain = m_dwCNGGain; dwCNGSW = m_dwCNGSW;}

private:

	DWORD m_dwCNGGain;
	DWORD m_dwCNGSW;
	float m_fCNGGain;
	int m_nCBIndexGainBak;


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCNG)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCNG_H__1D9A6730_C26C_433C_97C6_588EC2F00AF2__INCLUDED_)
