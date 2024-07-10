// CoolEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CoolEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolEdit

CCoolEdit::CCoolEdit()
{
	m_bIsSMSInput  = FALSE;
	m_bEnableHover = TRUE;
	m_bFocus       = FALSE;
	m_bHover       = FALSE;
	m_bDisPaste    = FALSE;
	m_bFirstMax    = TRUE;

	m_nLimitText    = -1;
}

CCoolEdit::~CCoolEdit()
{
}

BEGIN_MESSAGE_MAP(CCoolEdit, CEdit)
	//{{AFX_MSG_MAP(CCoolEdit)
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_NCPAINT()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolEdit message handlers

void CCoolEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnMouseMove(nFlags, point);

	if( !m_bHover )
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize      = sizeof(TRACKMOUSEEVENT);
		tme.hwndTrack   = m_hWnd;
		tme.dwFlags     = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 4;
		
		_TrackMouseEvent(&tme);
	}
}

LRESULT CCoolEdit::OnMouseHover(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_bHover = TRUE;
	
	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	return 0L;
}

LRESULT CCoolEdit::OnMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_bHover = FALSE;
	
	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

	return 0L;
}

void CCoolEdit::OnLButtonDown( UINT nFlags, CPoint point )
{
	//Invalidate();

	CEdit::OnLButtonDown(nFlags, point);
}

void CCoolEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	m_bFocus = TRUE;

	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}

void CCoolEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	m_bFocus = FALSE;

	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}

void CCoolEdit::OnNcPaint() 
{
	// TODO: Add your message handler code here
	if( m_bEnableHover )
	{
		CWindowDC dc(this);

		CRect rect;
		GetWindowRect(rect);
		
		rect.OffsetRect( -rect.left, -rect.top );

		if( m_bHover || m_bFocus )
		{
			dc.Draw3dRect( rect, RGB(0,0,0), RGB(0,0,0) );
		}
		else
		{
			dc.DrawEdge(rect, EDGE_SUNKEN , BF_FLAT | BF_RECT);
		}

		Invalidate();
	}
	else
	{
		CEdit::OnNcPaint();
	}
	// Do not call CEdit::OnNcPaint() for painting messages
}

BOOL CCoolEdit::PreTranslateMessage(MSG* pMsg)
{
	return CEdit::PreTranslateMessage(pMsg);
}

LRESULT CCoolEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_bDisPaste && message == WM_PASTE )
		return 0L;

	return CEdit::WindowProc(message, wParam, lParam);
}

void CCoolEdit::SetLimitText(UINT nMax)
{
	m_nLimitText = nMax;

	CEdit::SetLimitText(nMax);
}

void CCoolEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
#ifndef  _UNICODE

BOOL CCoolEdit::HasUcs2Char(LPCSTR pStr) const
{
	_ASSERTE( pStr != NULL );

	LPCSTR pNext = NULL;
	while( *pStr )
	{
		pNext = CharNextExA(CP_ACP,pStr,0);
		if( pNext - pStr > 1 )
			return TRUE;

		pStr = pNext;
	}

	return FALSE;
}
#else
BOOL CCoolEdit::HasUcs2Char(LPCTSTR pStr) const
{
	_ASSERTE( pStr != NULL );
    char *pChar = (char *)pStr;
	int nLen = _tcslen(pStr);
	int i=0;
	for(i=0; i<nLen*2; i=i+2)
	{
		if(!(pChar[i] < (UCHAR)0x80 && pChar[i+1] == 0)) 
			break;
	}
	if(i>=nLen*2)
	{
		return FALSE;
	}
	
	return TRUE;
}
#endif
BOOL CCoolEdit::HasUcs2Char() const
{
	CString strText;
	GetWindowText(strText);

	return HasUcs2Char(strText);
}

int CCoolEdit::GetFullCharNum(LPCTSTR pStr, int /*nLen*/)
{
	_ASSERTE( pStr != NULL );

	int     nNum  = 0;
	LPCTSTR pNext = NULL;
	while( *pStr )
	{
		pNext = CharNext(pStr);
		nNum++;
		pStr = pNext;
	}

	return nNum;
}

int CCoolEdit::TruncateFullChar(LPTSTR pszBuf, int nFullCharNum)
{
	_ASSERTE( pszBuf != NULL && nFullCharNum >= 0 );

	int    nNum  = 0;
	LPTSTR pNext = NULL;
	while( *pszBuf )
	{
		if( nNum == nFullCharNum )
		{
			*pszBuf = '\0';
			break;
		}

		pNext = CharNext(pszBuf);
		nNum++;
		pszBuf = pNext;
	}

	return nNum;
}

void CCoolEdit::OnUpdate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	if( m_nLimitText > 0 )
	{
		static const int nMaxTextLen = 1024;
		TCHAR szText[ nMaxTextLen ];
		GetWindowText(szText, nMaxTextLen);

		int nTxtLen      = _tcslen(szText);
		int nFullCharNum = GetFullCharNum(szText, nTxtLen);
	
		int nHarfLimit = m_bIsSMSInput ? ((m_nLimitText - 95) / 2 ) : (m_nLimitText / 2);
		
 		if(nFullCharNum < nHarfLimit && HasUcs2Char())
 		{
 			m_bFirstMax = TRUE;
 		}

		if( nFullCharNum == nHarfLimit && HasUcs2Char() && m_bFirstMax)
		{
			m_strMax = szText;
			m_bFirstMax = FALSE;
		}

		if( nFullCharNum > nHarfLimit && HasUcs2Char() )
		{
			if(m_bFirstMax)
			{		
				TruncateFullChar(szText, nHarfLimit);
				m_strText = szText;
			}
			else
			    m_strText = m_strMax;

			int nSelStart, nSelEnd;
			GetSel(nSelStart, nSelEnd);
			SetWindowText(m_strText);
			SetSel(nSelStart-1, nSelEnd-1);
	
		}
		else
		{
			m_strText = szText;
		}

		Invalidate();
	}
}
