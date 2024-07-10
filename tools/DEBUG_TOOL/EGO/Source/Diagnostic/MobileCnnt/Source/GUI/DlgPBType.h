#if !defined(AFX_DLGPBTYPE_H__BE22BBCA_AF8A_4CEA_B85E_85B0978647CC__INCLUDED_)
#define AFX_DLGPBTYPE_H__BE22BBCA_AF8A_4CEA_B85E_85B0978647CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPBType.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPBType dialog

class CDlgPBType : public CDialog
{
// Construction
public:
	CDlgPBType(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPBType)
	enum { IDD = IDD_DLG_PB_TYPE };
	CComboBox m_boxType;
	UINT m_uPBUsed;
	UINT m_uPBMax;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
private:
	void SendCheckRequest();
	BOOL SendSetRequest(DWORD dwType);
	void ParseCPBS(CString strCPBS);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPBType)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPBType)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboPbType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPBTYPE_H__BE22BBCA_AF8A_4CEA_B85E_85B0978647CC__INCLUDED_)
