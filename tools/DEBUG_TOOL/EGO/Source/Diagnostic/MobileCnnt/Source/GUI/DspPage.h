#if !defined(AFX_DSPPAGE_H__E62CA4F8_A3FD_46DB_B1D8_207AB04E67D8__INCLUDED_)
#define AFX_DSPPAGE_H__E62CA4F8_A3FD_46DB_B1D8_207AB04E67D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DspPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDspPage dialog

class CDspPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDspPage)

// Construction
public:
	CDspPage();
	~CDspPage();

// Dialog Data
	//{{AFX_DATA(CDspPage)
	enum { IDD = IDD_PAGE_DSP };
	UINT m_uParam1;
	UINT m_uParam2;
	UINT m_uParam3;
	UINT m_uParam4;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

public:
	void ClearAll();

private:
	CBrush  m_brshDlgBckGrnd;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDspPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDspPage)
	afx_msg void OnBtnApply();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPPAGE_H__E62CA4F8_A3FD_46DB_B1D8_207AB04E67D8__INCLUDED_)
