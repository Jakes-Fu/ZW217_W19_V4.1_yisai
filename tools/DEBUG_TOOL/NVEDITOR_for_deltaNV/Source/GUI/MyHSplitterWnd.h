#if !defined(AFX_MYHSPLITTERWND_H__B8F4556D_BC39_4264_B184_A3C1714F3593__INCLUDED_)
#define AFX_MYHSPLITTERWND_H__B8F4556D_BC39_4264_B184_A3C1714F3593__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyHSplitterWnd.h : header file
//
enum
{
	E_EDITOR_PANE = 0,
	E_CUST_PANE = 1,
	E_COUNT_PANE
};
/////////////////////////////////////////////////////////////////////////////
// CMyHSplitterWnd window

class CMyHSplitterWnd : public CSplitterWnd
{
// Construction
public:
	CMyHSplitterWnd();

// Attributes
public:	

// Operations
public:
	void HideRightCol();
	void ShowRightCol();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyHSplitterWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyHSplitterWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyHSplitterWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MyHSplitterWnd_H__B8F4556D_BC39_4264_B184_A3C1714F3593__INCLUDED_)
