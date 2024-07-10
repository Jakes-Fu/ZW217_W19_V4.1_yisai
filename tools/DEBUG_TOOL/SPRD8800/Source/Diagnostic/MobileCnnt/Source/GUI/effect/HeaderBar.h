#if !defined(AFX_HEADERBAR_H__8D7889BF_967A_4827_90D7_D39A0865F69A__INCLUDED_)
#define AFX_HEADERBAR_H__8D7889BF_967A_4827_90D7_D39A0865F69A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaderBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeaderBar window

class CHeaderBar : public CControlBar
{
// Construction
public:
	CHeaderBar();
    virtual ~CHeaderBar();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaderBar)
	public:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL /*bStretch*/, BOOL /*bHorz*/);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColor( COLORREF clrForeGrnd, COLORREF clrBackGrnd );
	void SetText( LPCTSTR lpText );
	// Generated message map functions
protected:
	//{{AFX_MSG(CHeaderBar)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int m_nHeight;
	int m_nWidth;

	COLORREF m_clrBackGrnd;
	COLORREF m_clrForeGrnd;

	CFont    m_font;
	CBrush   m_brush;

	CString  m_strText;

	static const COLORREF CLR_FORE_GRND;
	static const COLORREF CLR_BACK_GRND;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADERBAR_H__8D7889BF_967A_4827_90D7_D39A0865F69A__INCLUDED_)
