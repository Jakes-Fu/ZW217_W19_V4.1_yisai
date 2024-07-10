#if !defined(AFX_EDITEX_H__491E6B45_E6B4_42AC_BFC0_6579B8EBF1F3__INCLUDED_)
#define AFX_EDITEX_H__491E6B45_E6B4_42AC_BFC0_6579B8EBF1F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditEx window

class CEditEx : public CEdit
{
// Construction
public:
	CEditEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditEx)
	//afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITEX_H__491E6B45_E6B4_42AC_BFC0_6579B8EBF1F3__INCLUDED_)
