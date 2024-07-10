#if !defined(AFX_COOLLISTCTRL_H__76A7A50A_AA16_4B70_BF2E_CD59F88E8665__INCLUDED_)
#define AFX_COOLLISTCTRL_H__76A7A50A_AA16_4B70_BF2E_CD59F88E8665__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolListCtrl window

class CCoolListCtrl : public CListCtrl
{
// Construction
public:
	CCoolListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCoolListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCoolListCtrl)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBegintrack(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLLISTCTRL_H__76A7A50A_AA16_4B70_BF2E_CD59F88E8665__INCLUDED_)
