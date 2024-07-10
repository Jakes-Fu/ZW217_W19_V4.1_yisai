#if !defined(AFX_COLORSTATIC_H__0DB3E580_FAE7_4B02_B5A0_528FE9AC7148__INCLUDED_)
#define AFX_COLORSTATIC_H__0DB3E580_FAE7_4B02_B5A0_528FE9AC7148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorStatic window

class CColorStatic : public CStatic
{
// Construction
public:
	CColorStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBkColor(COLORREF colorBK);
	virtual ~CColorStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_colorBK;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSTATIC_H__0DB3E580_FAE7_4B02_B5A0_528FE9AC7148__INCLUDED_)
