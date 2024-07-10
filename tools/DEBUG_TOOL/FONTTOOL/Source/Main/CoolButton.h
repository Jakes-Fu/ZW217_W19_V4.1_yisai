#if !defined(AFX_COOLBUTTON_H__A65367DA_4499_4213_8C2E_8CBE9C8EDC97__INCLUDED_)
#define AFX_COOLBUTTON_H__A65367DA_4499_4213_8C2E_8CBE9C8EDC97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolButton window

class CCoolButton : public CButton
{
// Construction
public:
	CCoolButton();

// Attributes
public:
	

// Operations
public:

	BOOL LoadBitmaps(UINT nNormal, UINT nHover, UINT nClicked = 0, UINT nDisabled = 0);
	BOOL LoadBitmaps(LPCTSTR pszNormal, LPCTSTR pszHover, LPCTSTR pszClicked = NULL, LPCTSTR pszDisabled = NULL);
    void SetColors(COLORREF clrNormal, COLORREF clrHover, COLORREF clrClicked = 0,  COLORREF clrDisabled = 0);

	void SetShowText(BOOL bShow);
	void SetAutoSize(BOOL bAutoSize);
	void SetClicked(BOOL bClicked);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void AutoSize(void);
	virtual ~CCoolButton();

	// Generated message map functions
protected:
	void DrawBackGround(CDC * pDC, COLORREF clrBckGrnd);
	void DrawBackGround(CDC * pDC, CBitmap * pBmp);
	void DrawText(CDC * pDC);
	//{{AFX_MSG(CCoolButton)
	afx_msg void OnMouseMove(UINT nFlags, const CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseHover(WPARAM /*wParam*/, LPARAM /*lParam*/);
	afx_msg LRESULT OnMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/);

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bHover;
	BOOL m_bClicked;
	BOOL m_bAutoSize;
	BOOL m_bShowText;
	BOOL m_bUsedColors;

	CBitmap m_bmpNormal;
	CBitmap m_bmpHover;
	CBitmap m_bmpClicked;
	CBitmap m_bmpDisabled;

	COLORREF m_clrNormal;
	COLORREF m_clrHover;
	COLORREF m_clrClicked;
	COLORREF m_clrDisabled;
	
//	static const COLORREF CLR_DISABLE_TEXT;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLBUTTON_H__A65367DA_4499_4213_8C2E_8CBE9C8EDC97__INCLUDED_)
