#if !defined(AFX_COOLEDIT_H__12CF1A1F_B3E1_4116_8957_2BA54792BD07__INCLUDED_)
#define AFX_COOLEDIT_H__12CF1A1F_B3E1_4116_8957_2BA54792BD07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolEdit window

class CCoolEdit : public CEdit
{
// Construction
public:
	CCoolEdit();

// Attributes
public:
	BOOL m_bFocus;
// Operations
public:

	void SetSMSInput(BOOL bSMS = TRUE)     { m_bIsSMSInput = bSMS; }
	void SetLimitText(UINT nMax);
	void EnableHover(BOOL bEnable = TRUE)  { m_bEnableHover = bEnable; }
	void DisablePaste( BOOL bDis = TRUE )  { m_bDisPaste    = bDis;    }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL HasUcs2Char(void) const;
#ifndef _UNICODE
	BOOL HasUcs2Char(LPCSTR pStr) const;
#else
	BOOL HasUcs2Char(LPCTSTR pStr) const;
#endif
	int TruncateFullChar( LPTSTR pszBuf, int nFullCharNum );
    int GetFullCharNum( LPCTSTR pStr, int /*nLen*/ );
	virtual ~CCoolEdit();

	// Generated message map functions
protected:
	
	//{{AFX_MSG(CCoolEdit)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdate();
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	//}}AFX_MSG
	afx_msg LRESULT OnMouseHover(WPARAM /*wParam*/, LPARAM /*lParam*/);
	afx_msg LRESULT OnMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/);

	DECLARE_MESSAGE_MAP()
private:

	CString m_strText;

	CString m_strMax;
	BOOL    m_bFirstMax;

	BOOL m_bIsSMSInput;
	BOOL m_bEnableHover;
	BOOL m_bHover;
	BOOL m_bDisPaste;

	int  m_nLimitText;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLEDIT_H__12CF1A1F_B3E1_4116_8957_2BA54792BD07__INCLUDED_)
