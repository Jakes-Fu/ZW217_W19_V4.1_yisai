// XListCtrl.h  Version 1.4
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XLISTCTRL_H
#define XLISTCTRL_H

#include <map>
///////////////////////////////////////////////////////////////////////////////
// CXListCtrl class

class CXListCtrl : public CListCtrl
{
// Construction
public:
	CXListCtrl();
	virtual ~CXListCtrl();

// Attributes
private:

// Operations
public:
	void	SetItemColors(int nItem,								//+++
						  COLORREF crText, 
						  COLORREF crBackground);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXListCtrl)
public:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:

	std::map<WORD, DWORD>m_mapTextColor;
	std::map<WORD, DWORD>m_mapBKColor;

	// Generated message map functions
protected:
	//{{AFX_MSG(CXListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XLISTCTRL_H
