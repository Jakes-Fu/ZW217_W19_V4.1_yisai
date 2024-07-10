// HexEdit.cpp : implementation file
//

#include "stdafx.h"
#include "HexEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum EDIT_MODE			// m_editMode flags
{
	EDIT_BYTES  = 1,
	EDIT_INSERT = 2,
};

const int ID_TIMER = 1;

/*lint -e414 */

/////////////////////////////////////////////////////////////////////////////
// CHexEdit

CHexEdit::CHexEdit()
{
   	m_selStart = m_selEnd = -1;
	m_editPos = m_viewPos = 0;
	m_editMode = EDIT_BYTES; // | EDIT_INSERT;
	m_bMouseDown = m_bMouseMove = FALSE;
	m_bTimer = FALSE;
	m_dataModified = FALSE;
	m_dwStartAddr = 0;
    m_bBigEndian = FALSE;

    m_showAddress = TRUE;
	m_showAscii = TRUE;
	m_digitsInData = 2;
	m_digitsInAddress = 8;
	m_columns = 0; // Auto-column
	m_fontHeight = 10;
	m_allowChangeSize = FALSE;

    m_clrBack = RGB( 255, 255, 255 );
    m_clrFont = RGB( 0, 0, 0 );

    m_bDataModified = FALSE;
    m_bRealShowAscii = FALSE;
    m_nHorzScroll = 0;
    m_posMouseDown = 0;
    m_nEditDigit = 0;
    m_addrMargin = 0;
    m_asciiMargin = 0;
}

CHexEdit::~CHexEdit()
{
}

BEGIN_MESSAGE_MAP(CHexEdit, CWnd)
	//{{AFX_MSG_MAP(CHexEdit)
	ON_WM_PAINT()
    ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
  	ON_WM_CONTEXTMENU()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEdit message handlers

BOOL CHexEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

    if ( WM_CHAR == pMsg->message )
    {
        SendMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
        return TRUE;
    }
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CHexEdit::OnShowAddressChanged() 
{
    if( m_aBuffer.GetSize() != 0 )
	{
		// do following only if m_data has been set 
		m_nHorzScroll = 0;
		RecalcLayout();
		Invalidate();
	}
}

void CHexEdit::OnShowAsciiChanged() 
{
	if ( m_digitsInData == 2 )
	{
		m_nEditDigit = 0;
		if ( !m_showAscii )
        {
			m_editMode |= EDIT_BYTES;
        }

        if( m_aBuffer.GetSize() != 0 )
		{
			// do following only if m_data has been set 
			m_nHorzScroll = 0;
			RecalcLayout();
			ChangeEditPos(0,0);
			Invalidate();
		}
	}
}

BOOL CHexEdit::GetEndianMode()
{
    return m_bBigEndian;
}

void CHexEdit::SetEndianMode( BOOL bBigEndian )
{
    if ( m_bBigEndian != bBigEndian )
    {
        m_bBigEndian = bBigEndian;
        Invalidate();
    }
}

void CHexEdit::GetColor( COLORREF &clrBack, COLORREF &clrFont )
{
    clrBack = m_clrBack;
    clrFont = m_clrFont;
}

void CHexEdit::SetColor( COLORREF clrBack, COLORREF clrFont )
{
    m_clrBack = clrBack;
    m_clrFont = clrFont;
}

short CHexEdit::GetColumns() 
{
	return m_columns;
}

void CHexEdit::SetColumns(short nNewValue) 
{
	if ( nNewValue < 0 )
	{
		return;
	}
	m_columns = nNewValue;

    if( m_aBuffer.GetSize() != 0 )
    {
		// do following only if m_data has been set 
		m_nHorzScroll = 0;
		RecalcLayout();
		Invalidate();
	}
}

void CHexEdit::SetData( void* pData, int nSize, unsigned int dwStartAddr /* = 0 */ )
{
    ASSERT( pData != NULL );
    ASSERT( nSize > 0 );

    m_aBuffer.RemoveAll();
    m_aBuffer.SetSize(nSize);
    memcpy(m_aBuffer.GetData(),pData,nSize);

    m_dwStartAddr = dwStartAddr;
	m_nEditDigit = 0;
	m_selStart = m_selEnd = -1;
	m_editPos = m_viewPos = m_nHorzScroll = 0;
	m_editMode |= EDIT_BYTES;
	m_dataModified = FALSE;

	RecalcLayout();
	Invalidate();
}

void* CHexEdit::GetData( int &nSize )
{
    nSize = m_aBuffer.GetSize();
    return m_aBuffer.GetData();
}

short CHexEdit::GetFontHeight() 
{
	return m_fontHeight;
}

void CHexEdit::SetFontHeight(short nNewValue) 
{
	if ( nNewValue < 8 )
	{
		return;
	}

	m_fontHeight = nNewValue;

    if( m_aBuffer.GetSize() != 0 )
	{
		// do following only if m_data has been set 
		m_nHorzScroll = 0;
		RecalcLayout();
		Invalidate();
	}
}

short CHexEdit::GetDigitsInAddress() 
{
	return m_digitsInAddress;
}

void CHexEdit::SetDigitsInAddress(short nNewValue) 
{
	if ( nNewValue != 4 && nNewValue != 8 )
	{
		return;
	}

	m_digitsInAddress = nNewValue;

    if( m_aBuffer.GetSize() != 0 )
	{
		// do following only if m_data has been set 
		m_nHorzScroll = 0;
		RecalcLayout();
		Invalidate();
	}
}

short CHexEdit::GetDigitsInData() 
{
	return m_digitsInData;
}

void CHexEdit::SetDigitsInData(short nNewValue) 
{
	if ( nNewValue != 2 && nNewValue != 4 && nNewValue != 8 )
	{
		return;
	}
	m_digitsInData = nNewValue;
	m_nEditDigit = 0;
	m_selStart = m_selEnd = -1;
	m_editPos = m_viewPos = m_nHorzScroll = 0;
	m_editMode |= EDIT_BYTES;

	if( m_aBuffer.GetSize() != 0 )
    {
		// do following only if m_data has been set 
		RecalcLayout();
		Invalidate();
	}
}

void CHexEdit::ShowAddress(BOOL bShow)
{
    if ( m_showAddress != bShow )
    {
        m_showAddress = bShow;
        OnShowAddressChanged();
    }        
}

void CHexEdit::ShowAscii(BOOL bShow)
{
    if(m_showAscii != bShow)
    {
        m_showAscii = bShow;
        OnShowAddressChanged();
    }
}

void CHexEdit::EnableChangingDataSize( BOOL bEnable )
{
    m_allowChangeSize = bEnable;
}

void CHexEdit::InvalidateEditRect(int nEditPos)
{
	CRect rcInvalid;
	int nPos = nEditPos - m_viewPos;
	ASSERT ( nPos >= 0 && nPos < m_columns*m_charCountWindow.cy );

	// invalidate rect of the number
	rcInvalid.left = (nPos % m_columns * (m_digitsInData+1) + m_addrMargin - m_nHorzScroll)*m_cellSize.cx+m_cellSize.cx/2;
	rcInvalid.top = nPos / m_columns * m_cellSize.cy;
	rcInvalid.right = rcInvalid.left + (m_digitsInData+1)*m_cellSize.cx;
	rcInvalid.bottom = rcInvalid.top + m_cellSize.cy;
	InvalidateRect(rcInvalid);

	if ( m_bRealShowAscii )
	{
		// invalidate rect of corresponding ASCII char
		rcInvalid.left = (nPos % m_columns + m_asciiMargin - m_nHorzScroll)*m_cellSize.cx+m_cellSize.cx/2;
		rcInvalid.top = nPos / m_columns * m_cellSize.cy;
		rcInvalid.right = rcInvalid.left + m_cellSize.cx;
		rcInvalid.bottom = rcInvalid.top + m_cellSize.cy;
		InvalidateRect(rcInvalid);
	}
}

void CHexEdit::DeleteSelection()
{
	if ( !IsSelectionEmpty())
	{
		DeleteData ( m_selStart, m_selEnd-m_selStart+1 );
		m_editPos = m_selStart;
		m_dataModified = TRUE;

		ClearSelection();
		NormalizeEditPos();
		PlaceCaret();
	}
}

void CHexEdit::RecalcLayout()
{
	if ( m_hWnd == 0 ) return;		// invisible window

	if ( m_aBuffer.GetSize()+m_dwStartAddr > 0x10000 )
		m_digitsInAddress = 8;

	int nColumns = m_columns;
	CFont font;
	font.CreatePointFont ( m_fontHeight * 10, _T("Courier") );

	CDC* pdc = GetDC();
	ASSERT ( pdc );

	CFont* oldFont = pdc->SelectObject ( &font );

	// determine char size
	m_cellSize = pdc->GetTextExtent(_T("0"));
	pdc->LPtoDP ( &m_cellSize );
	pdc->SelectObject ( oldFont );
	ReleaseDC ( pdc );

	GetClientRect ( m_rcClient );

	// compute entire rect of the control, including scroll bars
	if (GetStyle() & WS_VSCROLL)
		m_rcClient.right += ::GetSystemMetrics(SM_CXVSCROLL);
	if (GetStyle() & WS_HSCROLL)
		m_rcClient.bottom += ::GetSystemMetrics(SM_CYHSCROLL);

	// before calculations
	BOOL bShowSbVert = FALSE;	// need to show vertical scroll bar?
	BOOL bShowSbHorz = FALSE;	// need to show horizontal scroll bar?

	// after calculations
	BOOL bShowSbVert2 = FALSE;	
	BOOL bShowSbHorz2 = FALSE;	

	m_bRealShowAscii = (m_digitsInData == 2 && m_showAscii);

	do
	{
		bShowSbVert = bShowSbVert2;
		bShowSbHorz = bShowSbHorz2;

		// first pass - compute params when scrollbars are hidden
		// next passes (0, 1, or 2) - recalulate params if client rect was shrunk after showing scrollbars
		m_charCountWindow.cx = (m_rcClient.Width() - m_cellSize.cx/2) / m_cellSize.cx;
		m_charCountWindow.cy = m_rcClient.Height() / m_cellSize.cy;
		
		if ( m_showAddress )
			m_addrMargin = m_digitsInAddress+2;
		else
			m_addrMargin = 0;
		
		if ( nColumns == 0 )
		{
			// auto columns
			if ( m_bRealShowAscii )
				m_columns = short((m_charCountWindow.cx - m_addrMargin - 1)/(m_digitsInData+2));
			else
				m_columns = short((m_charCountWindow.cx - m_addrMargin+1)/(m_digitsInData+1));
			if ( m_columns <= 0 )
				m_columns = 1;
		}
		m_asciiMargin = m_charCountView.cx = m_columns * (m_digitsInData+1) + m_addrMargin;
		if ( m_bRealShowAscii )
		{
			m_charCountView.cx += m_columns * (m_digitsInData/2) + 1;
			m_asciiMargin++;
		}
		else
			m_charCountView.cx--;
		m_charCountView.cy = m_aBuffer.GetSize() / (m_columns*(m_digitsInData/2)) + 1;

		if ( m_charCountView.cx > m_charCountWindow.cx && !bShowSbHorz )
		{
			m_rcClient.bottom -= ::GetSystemMetrics(SM_CYHSCROLL);
			bShowSbHorz2 = TRUE;
		}
		if ( m_charCountView.cy > m_charCountWindow.cy && !bShowSbVert )
		{
			m_rcClient.right -= ::GetSystemMetrics(SM_CXVSCROLL);
			bShowSbVert2 = TRUE;
		}
	} while ( bShowSbHorz != bShowSbHorz2 || bShowSbVert != bShowSbVert2 );

	if ( m_digitsInData != 2 )
    {
		// rounding data length on (m_digitsInData/2)
		m_aBuffer.SetSize( (m_aBuffer.GetSize()+(m_digitsInData/2)-1)/(m_digitsInData/2)*(m_digitsInData/2));
    }

	m_viewPos = m_viewPos / m_columns * m_columns;
	NormalizeEditPos();
	UpdateScrollBars();

	if (!(GetStyle() & WS_VSCROLL))
    {
		m_viewPos = 0;
    }
}

void CHexEdit::PlaceCaret()
{
	if ( GetFocus()->m_hWnd == m_hWnd )	// does this window have focus?
	{
		POINT pt;
		
		if ( !IsSelectionEmpty() && m_editPos == m_selEnd+1 )
		{
			// place caret just after the selection
			if ( m_editMode & EDIT_BYTES )
			{
				pt.x = m_selEnd % m_columns * (m_digitsInData+1) + m_digitsInData + 1;
				if ( m_showAddress )
					pt.x += m_addrMargin;
			}
			else
				pt.x = m_asciiMargin + m_selEnd % m_columns + 1;
			
			pt.y = (m_selEnd - m_viewPos)/m_columns;
		}
		else
		{
			if ( m_editMode & EDIT_BYTES )
			{
				pt.x = m_editPos % m_columns * (m_digitsInData+1) + m_nEditDigit;
				if ( m_showAddress )
					pt.x += m_addrMargin;
			}
			else
				pt.x = m_asciiMargin + m_editPos % m_columns;
			
			pt.y = (m_editPos - m_viewPos)/m_columns;
		}
		pt.x = (pt.x - m_nHorzScroll)*m_cellSize.cx+m_cellSize.cx/2-1;

		pt.y *= m_cellSize.cy;
		SetCaretPos ( pt );
	}
}

void CHexEdit::ChangeEditPos(int dx, int dy, BOOL bRepaint /* = FALSE */)
{
	m_nEditDigit = 0;
	m_editPos += dy*m_columns + dx;
	if ( NormalizeEditPos())
		bRepaint = TRUE;

	if ( UpdateScrollBars ())
		bRepaint = TRUE;

	if ( bRepaint )
		Invalidate();
	PlaceCaret();
}

void CHexEdit::ChangeEditPos(CPoint pt)
{
	pt.x = (pt.x - m_cellSize.cx/2) / m_cellSize.cx;
	pt.y /= m_cellSize.cy;

	pt.x += m_nHorzScroll;

	if ( pt.x >= m_asciiMargin && m_bRealShowAscii && !m_bMouseMove || 
		m_bMouseMove && !(m_editMode & EDIT_BYTES))	
		// click in the ascii field or extending selection in ascii field
		
		/* if selection was started in ascii, we never shouldn't switch 
		   to hex field until selection is finished
		   The same, when selection started in hex field
		 */
	{
		if ( pt.x < m_asciiMargin )
			pt.x = m_asciiMargin;
		else
		if ( pt.x >= m_asciiMargin + m_columns*m_digitsInData/2 )
			pt.x = m_asciiMargin + m_columns*m_digitsInData/2 - 1;
		m_editPos = m_viewPos + (pt.x-m_asciiMargin) + pt.y * m_columns;
		m_editMode &= ~EDIT_BYTES;
	}
	else
	{
		m_editMode |= EDIT_BYTES;
		if ( pt.x >= m_asciiMargin )
			pt.x = m_asciiMargin - 1;
		pt.x -= m_addrMargin;
		if ( pt.x < 0 )	// clicked in address area
			pt.x = 0;
		if ( pt.x >= m_columns*(m_digitsInData+1))
			pt.x = (m_columns-1)*(m_digitsInData+1);
		
		m_nEditDigit = pt.x % (m_digitsInData+1);
		if ( m_nEditDigit == m_digitsInData )
		{
			m_nEditDigit = m_digitsInData-1;
		}
		m_editPos = m_viewPos + pt.x / (m_digitsInData+1) + pt.y * m_columns;
	}

	NormalizeEditPos();
	UpdateScrollBars();
}

BOOL CHexEdit::NormalizeEditPos()
{
	BOOL bRepaint = FALSE;
	DWORD dwDataSize = m_aBuffer.GetSize()/(m_digitsInData/2);

	if ( m_editPos < 0 )
		m_editPos = 0;
	else if ( DWORD(m_editPos) >= dwDataSize )
	{
		m_editPos = dwDataSize;
		m_nEditDigit = 0;
	}

	// change m_viewPos if m_editPos is moved out of the visible area
	if ( m_viewPos > m_editPos )
	{
		m_viewPos = m_editPos / m_columns * m_columns;
		bRepaint = TRUE;
	}
	else if ( m_viewPos+m_columns*m_charCountWindow.cy <= m_editPos )
	{
		m_viewPos = (m_editPos / m_columns - m_charCountWindow.cy + 1) * m_columns;
		bRepaint = TRUE;
	}

	// if m_viewPos is too near to the end of data, decrease m_viewPos so entire page is displayed
	// (this may happen after deleting large selection or changing properties)
	if ( m_viewPos > m_aBuffer.GetSize() / (m_digitsInData/2) - m_columns*(m_charCountWindow.cy-1))
	{
		m_viewPos = m_aBuffer.GetSize() / (m_digitsInData/2) / m_columns * m_columns - m_columns*(m_charCountWindow.cy-1);
		if ( m_viewPos < 0 ) m_viewPos = 0;
	}

	int nPosX;		// x coordinate of caret in view
	if ( m_editMode & EDIT_BYTES )
		nPosX = m_addrMargin + m_editPos % m_columns * (m_digitsInData+1);
	else	// EDIT_ASCII
		nPosX = m_asciiMargin + m_editPos % m_columns;
	
	if ( nPosX < m_nHorzScroll )
	{
		m_nHorzScroll = nPosX;
		bRepaint = TRUE;
	}
	else
	{
		if ( m_editMode & EDIT_BYTES ) 
			nPosX += m_digitsInData;	// to make entire number visible
		else
			nPosX++;
		if ( nPosX-m_nHorzScroll >= m_charCountWindow.cx )
		{
			m_nHorzScroll = nPosX - m_charCountWindow.cx;
			bRepaint = TRUE;
		}
	}
	return bRepaint;
}

void CHexEdit::InsertData(DWORD dwPos, DWORD dwCount/*=1*/)
{
	// convert to bytes
	dwPos *= (m_digitsInData/2);
	dwCount *= (m_digitsInData/2);

	DWORD dwArrSize = m_aBuffer.GetSize();
	ASSERT ( dwCount > 0 );
	ASSERT ( dwPos <= dwArrSize );

	if ( dwArrSize+dwCount <= dwArrSize || dwArrSize+dwCount >= 0x80000000 )
	{
		// overflow
		AfxThrowMemoryException ();
		return;
	}

	m_aBuffer.SetSize( dwArrSize + dwCount );

	if ( dwPos != dwArrSize )
	{
		BYTE* buf = m_aBuffer.GetData();

		memmove ( buf+dwPos+dwCount, buf+dwPos, dwArrSize-dwPos );
		memset ( buf+dwPos, 0, dwCount );
	}
	RecalcLayout();
}

void CHexEdit::DeleteData(DWORD dwPos, DWORD dwCount /*=1*/)
{
	// convert to bytes
	dwPos *= (m_digitsInData/2);
	dwCount *= (m_digitsInData/2);

	DWORD dwArrSize = m_aBuffer.GetSize();
	ASSERT ( dwCount > 0 );
	ASSERT ( dwPos < dwArrSize );

	if ( dwPos != dwArrSize-1 )
	{
		BYTE* buf = m_aBuffer.GetData();
		memmove ( buf+dwPos, buf+dwPos+dwCount, dwArrSize-dwPos-dwCount );
	}

	m_aBuffer.SetSize( dwArrSize - dwCount );
	RecalcLayout();
}

BOOL CHexEdit::UpdateScrollBars()
{
	BOOL bRepaint = FALSE;
	SCROLLINFO si, prevSI;

	si.cbSize = prevSI.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = m_charCountView.cy - 1;
	si.nPage = m_charCountWindow.cy;
	si.nPos = (m_viewPos+m_columns-1) / m_columns;

	if ( !bRepaint && ( !GetScrollInfo(SB_VERT, &prevSI, SIF_POS ) || si.nPos != prevSI.nPos ))
		// if scrollbar is about to show or its position changed, the control needs repainting
		bRepaint = TRUE;

	if(si.nMax >= int(si.nPage))
	{
		ShowScrollBar(SB_VERT);
	}
	else
	{
		ShowScrollBar(SB_VERT, FALSE);
	}
	VERIFY(SetScrollInfo(SB_VERT, &si, TRUE));

	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = m_charCountView.cx-1;
	si.nPage = m_charCountWindow.cx;
	si.nPos = m_nHorzScroll;

	if ( si.nMax > (int)si.nPage && si.nPos+int(si.nPage) > si.nMax )
		si.nPos = m_nHorzScroll = si.nMax - si.nPage + 1;
	if ( !bRepaint && ( !GetScrollInfo(SB_HORZ, &prevSI, SIF_POS ) || si.nPos != prevSI.nPos ))
		// if scrollbar is about to show or its position changed, the control needs repainting
		bRepaint = TRUE;

	if(si.nMax >= (int)si.nPage )
	{
		ShowScrollBar(SB_HORZ);
	}
	else
	{
		ShowScrollBar(SB_HORZ, FALSE);
	}

	VERIFY(SetScrollInfo(SB_HORZ, &si, TRUE));
	return bRepaint;
}

inline void CHexEdit::ClearSelection()
{
	m_selStart = m_selEnd = -1;
	Invalidate();
}

inline BOOL CHexEdit::IsSelectionEmpty()
{
	return ( m_selStart == -1 );
}

void CHexEdit::RecalcSelection(CPoint point)
{
	if ( m_aBuffer.GetSize() == 0 )
		return;
	if ( m_bMouseDown )
	{
		int nPrevSelStart = m_selStart, nPrevSelEnd = m_selEnd;

		ChangeEditPos ( point );
		if ( m_posMouseDown < m_editPos )
		{
			if ( m_editPos*(m_digitsInData/2) == m_aBuffer.GetSize())
				m_editPos--;
			m_selStart = m_posMouseDown; 
			m_selEnd = m_editPos;
			m_editPos = m_selEnd;
			m_nEditDigit = 0;
		}
		else
		{
			m_selStart = m_editPos;
			m_selEnd = m_posMouseDown; 
			m_editPos = m_selStart;
			m_nEditDigit = 0;
		}
		if ( nPrevSelStart != m_selStart || nPrevSelEnd != m_selEnd )
			Invalidate();
	}
}


void CHexEdit::TextOutWithCheck(CDC *pdc, const CRect &rcInvalid, int x, int y, CString &strText)
{
	CRect rcText ( x, y, x+strText.GetLength()*m_cellSize.cx, y+m_cellSize.cy );

	if ( !((rcInvalid & rcText).IsRectEmpty()) )
		pdc->TextOut ( x, y, strText );
}

BOOL CHexEdit::EnterNumber()
{
	if ( !IsSelectionEmpty())
	{
		if ( (m_editMode & EDIT_INSERT) || m_allowChangeSize )
		{
			// delete current selection and insert 1 number
			m_editPos = m_selStart;
			DeleteSelection();
			InsertData ( m_editPos );
			Invalidate();
		}
		else
		{
			ClearSelection();
		}
	}
	else	// is m_editPos at the end?
		if ( m_editPos*(m_digitsInData/2) == m_aBuffer.GetSize())
		{
			if ( (m_editMode & EDIT_INSERT) || m_allowChangeSize )
			{
				InsertData ( m_editPos );
				if ( m_editPos % m_columns != m_columns-1)
					InvalidateEditRect (m_editPos);
				else
					Invalidate();	// we need to add new line
			}
			else
				return FALSE;
		}
		else if	((m_editMode & EDIT_INSERT) && m_nEditDigit == 0 )
			// insert 1 number
		{
			InsertData ( m_editPos );
			Invalidate();
		}
	m_dataModified = TRUE;
	return TRUE;
}

void CHexEdit::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
    if( m_aBuffer.GetSize() != 0 )
	{
		// do following only if m_data has been set 
		RecalcLayout();
		Invalidate();
	}
	
}

void CHexEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

    if( m_aBuffer.GetSize() == 0 )
		return;
	DestroyCaret ();
}

void CHexEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
    if( m_aBuffer.GetSize() == 0 )
		return;
	CreateSolidCaret ( (m_editMode & EDIT_INSERT) ? 0:m_cellSize.cx, 
		m_cellSize.cy );
	PlaceCaret();
	if ( !m_bMouseDown )
		ShowCaret();
}

void CHexEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BOOL bShift = ::GetKeyState(VK_SHIFT) & 0x80000000;
	BOOL bControl = ::GetKeyState(VK_CONTROL) & 0x80000000;

	if ( nChar == VK_TAB )
	{
		CWnd* pParent = GetParentOwner();
		if ( !pParent ) return;
		if ( bShift && !bControl )
		{
			::SendMessage(pParent->m_hWnd, WM_NEXTDLGCTL, 1, 0);
			return;
		}
		else if ( !bControl && !bShift )
		{
			::SendMessage(pParent->m_hWnd, WM_NEXTDLGCTL, 0, 0);
			return;
		}
	}

    if( m_aBuffer.GetSize() == 0 )
		return;
	
	if ( bShift && !bControl )
	{
		int prevEditPos = m_editPos;
		switch ( nChar )
			// SHIFT+arrows - modify selection
		{
		case VK_LEFT:
			ChangeEditPos ( -1,  0 );
			break;
		case VK_RIGHT:
			ChangeEditPos ( 1, 0 );
			break;
		case VK_UP:
			ChangeEditPos ( 0, -1 );
			break;
		case VK_DOWN:
			ChangeEditPos ( 0, 1 );
			break;
		case VK_PRIOR:
			ChangeEditPos ( 0, -m_charCountWindow.cy );
			break;
		case VK_NEXT:
			ChangeEditPos ( 0, m_charCountWindow.cy );
			break;
		case VK_HOME:
			if ( m_editMode & EDIT_BYTES )
				m_nHorzScroll = 0;
			if ( bControl )
				// select to the start of data
			{
				m_editPos = 0;
				ChangeEditPos ( 0, 0 );
			}
			else	// select to the beginning of the line
				ChangeEditPos ( -(m_editPos % m_columns),  0 );
			break;
		case VK_END:
			if ( bControl )
				// select to the end of data
			{
				m_editPos = m_aBuffer.GetSize()/(m_digitsInData/2);
				ChangeEditPos ( 0, 0 );
			}
			else	// select to the end of the line
				ChangeEditPos ( m_columns-(m_editPos % m_columns),  0 );
			break;
		case VK_INSERT:
			OnEditPaste();
			return;
		case VK_DELETE:
			OnEditCut();
			return;
		default:
			CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
			return;
		}
		// recalc selection margins
		if ( IsSelectionEmpty() )
		{
			if ( m_editPos < prevEditPos )
			{
				m_selStart = m_editPos;
				m_selEnd = prevEditPos-1;
			}
			else if ( m_editPos > prevEditPos )
			{
				m_selStart = prevEditPos;
				m_selEnd = m_editPos-1;
			}
		}
		else
		{
			if ( prevEditPos-1 == m_selEnd )
				m_selEnd = m_editPos-1;
			else
				m_selStart = m_editPos;
			if ( m_selStart == m_selEnd+1 )
				ClearSelection();
		}
		Invalidate();
	}
	else
	{
		if ( bControl && !bShift)
		{
			switch ( nChar )
			{
			case VK_HOME:
				m_editPos = 0;
				ChangeEditPos ( 0, 0 );
				break;
			case VK_END:
				m_editPos = m_aBuffer.GetSize();
				ChangeEditPos ( 0, 0 );
				break;
			case VK_INSERT:
				OnEditCopy();
				break;
			case 'A':
				OnEditSelectAll();
				break;
			case VK_TAB:	
				if ( m_digitsInData == 2 && m_bRealShowAscii )
				{	// switch hex/ascii
					m_editMode ^= EDIT_BYTES;
					ChangeEditPos(0,0);		// update scrollbars and caret pos
				}
				break;
			default:
				CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
				break;
			}
		}
		else if ( !bControl && !bShift )
		{
			switch ( nChar )
			{
			case VK_LEFT:
				ChangeEditPos ( -1,  0 );
				break;
			case VK_RIGHT:
				ChangeEditPos ( 1, 0 );
				break;
			case VK_UP:
				ChangeEditPos ( 0, -1 );
				break;
			case VK_DOWN:
				ChangeEditPos ( 0, 1 );
				break;
			case VK_PRIOR:
				ChangeEditPos ( 0, -m_charCountWindow.cy );
				break;
			case VK_NEXT:
				ChangeEditPos ( 0, m_charCountWindow.cy );
				break;
			case VK_HOME:
				if ( m_editMode & EDIT_BYTES )
					m_nHorzScroll = 0;
				ChangeEditPos ( -(m_editPos % m_columns),  0 );
				break;
			case VK_END:	// todo???: place caret at the end
				ChangeEditPos ( m_columns-(m_editPos % m_columns)-1,  0 );
				break;
			case VK_BACK:
				if ( (m_editMode & EDIT_INSERT) || m_allowChangeSize )
				{
					if ( IsSelectionEmpty() )
					{
						if ( m_editPos > 0 )
						{
							ChangeEditPos ( -1,  0, TRUE );
							DeleteData ( m_editPos );
							m_dataModified = TRUE;
						}
					}
					else
						DeleteSelection();
					Invalidate();
					PlaceCaret();
				}
				break;
			case VK_DELETE:
				if ( (m_editMode & EDIT_INSERT) || m_allowChangeSize )
				{
					if ( IsSelectionEmpty() )
					{
						if ( m_editPos < m_aBuffer.GetSize() )
						{
							DeleteData ( m_editPos );
							ChangeEditPos ( 0,  0, TRUE );
							m_dataModified = TRUE;
						}
					}
					else
					{
						DeleteSelection();
						Invalidate();
						PlaceCaret();
					}
				}
				else // if !EDIT_INSERT
				{
					if ( !IsSelectionEmpty() )
					{
						// zero selected bytes
						BYTE* buf = m_aBuffer.GetData();
						memset ( buf+m_selStart*(m_digitsInData/2), 0, (m_selEnd-m_selStart+1)*(m_digitsInData/2));
						ClearSelection();
					}
				}
				break;
			case VK_INSERT:
                if ( m_allowChangeSize )
                {
				    m_editMode ^= EDIT_INSERT;
				    CreateSolidCaret ( (m_editMode & EDIT_INSERT) ? 0:m_cellSize.cx, 
					    m_cellSize.cy );
				    ShowCaret();
				    PlaceCaret();
                }
				break;
			default:
				CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
				return;
			}

			if ( !IsSelectionEmpty() )
			{
				ClearSelection();
				Invalidate();
			}
		}
	}
}

void CHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if( m_aBuffer.GetSize() == 0 )
		return;

	if ( m_editMode & EDIT_BYTES )
	{
		if((nChar >= '0' && nChar <= '9') || (nChar >= 'a' && nChar <= 'f') || (nChar >= 'A' && nChar <= 'F'))
		{
			UINT b = nChar - '0';
			if(b > 9)
			{
				if (b <= 'F' - '0')
					b = 10 + nChar - 'A';
				else
					b = 10 + nChar - 'a';
			}
			
			if ( !EnterNumber())
				return;

            BYTE* buf = m_aBuffer.GetData();
			DWORD bufAddr = m_editPos*(m_digitsInData/2);
            if ( m_bBigEndian ) // Big Endian
            {
                BYTE bMask = (BYTE)(( 0 == m_nEditDigit % 2 ) ? 0x0F : 0xF0);
                b = ( 0 == m_nEditDigit % 2 ) ? b << 4 : b;
                int nOffset = bufAddr + m_nEditDigit / 2;
                buf[nOffset] = (BYTE)(( buf[nOffset] & bMask ) | b);
            }
            else // Little Endian
            {
                UINT mask = 0xFUL << (( m_digitsInData - m_nEditDigit - 1 )*4);
                *(DWORD*)(buf+bufAddr) = *(DWORD*)(buf+bufAddr) & ~mask | (b << (( m_digitsInData - m_nEditDigit - 1 )*4));
            }
            

            // @Liu Jun 2002-12-30
            // Notify the parent window (maybe it needs additional process)
            GetParent()->SendMessage( WM_HE_UNITDATA_CHANGED, m_dwStartAddr + bufAddr, *(DWORD*)(buf+bufAddr) );
			
			m_nEditDigit++;
			if (NormalizeEditPos())		
				Invalidate();			// cursor was outside visible area
			InvalidateEditRect ( m_editPos );
			if ( m_nEditDigit == m_digitsInData )
				ChangeEditPos(1, 0, FALSE);
			else
			{
				UpdateScrollBars();
				PlaceCaret();
			}
		}	
		else
			CWnd::OnChar(nChar, nRepCnt, nFlags);
	}
	else	// EDIT_ASCII
	{
		if ( nChar >= ' ' )
		{
			if ( !EnterNumber())
				return;

			BYTE* buf = m_aBuffer.GetData();
			buf[m_editPos] = BYTE(nChar);

            // @Liu Jun 2002-12-30
            // Notify the parent window (maybe it needs additional process)
            GetParent()->SendMessage( WM_HE_UNITDATA_CHANGED, m_dwStartAddr + m_editPos, buf[m_editPos] );

			if (NormalizeEditPos())		
				Invalidate();			// cursor was outside visible area
			else
				InvalidateEditRect ( m_editPos );
			ChangeEditPos(1, 0, FALSE);
		}
	}
}

UINT CHexEdit::OnGetDlgCode() 
{
	return CWnd::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTCHARS | DLGC_WANTTAB;
}

void CHexEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if( m_aBuffer.GetSize() == 0 )
		return;

    SetFocus();
	m_bMouseDown = TRUE;

	if ( !IsSelectionEmpty() )
		ClearSelection();

	if ( nFlags & MK_SHIFT )
	{
		if ( m_editPos*(m_digitsInData/2) == m_aBuffer.GetSize())
			m_posMouseDown = m_editPos - 1;
		else
			m_posMouseDown = m_editPos;
		RecalcSelection(point);
		m_bMouseMove = TRUE;
	}
	else
	{
		ChangeEditPos ( point );
		if ( m_editPos*(m_digitsInData/2) == m_aBuffer.GetSize())
			m_posMouseDown = m_editPos - 1;
		else
			m_posMouseDown = m_editPos;
	}
	SetCapture();
	HideCaret();
	CWnd::OnLButtonDown(nFlags, point);
}

void CHexEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    if( m_aBuffer.GetSize() == 0 )
		return;

	ChangeEditPos ( point );
	
	// select the number under cursor if it isn't at the end of data
	if ( m_editPos*(m_digitsInData/2) != m_aBuffer.GetSize())
	{
		m_selStart = m_selEnd = m_editPos;
		m_nEditDigit = 0;
		InvalidateEditRect(m_editPos);
		m_editPos++;
	}

	PlaceCaret();
	m_bMouseDown = FALSE;
	ReleaseCapture();

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CHexEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if( m_aBuffer.GetSize() == 0 )
		return;

	if ( m_bMouseDown )
	{
		if ( m_bMouseMove )
			RecalcSelection(point);
		ReleaseCapture();
		m_bMouseDown = FALSE;
		m_bMouseMove = FALSE;
		if ( m_bTimer )
		{
			KillTimer(ID_TIMER);
			m_bTimer = FALSE;
		}

		// place caret just after the selection
		if ( !IsSelectionEmpty() )
		{
			if ( m_editPos == m_selEnd )
				m_editPos++;
			m_nEditDigit = 0;
		}
	}
	PlaceCaret();
	ShowCaret();
	CWnd::OnLButtonUp(nFlags, point);
}

void CHexEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
    if( m_aBuffer.GetSize() == 0 )
		return;
	if ( m_bMouseDown )
	{
		m_bMouseMove = TRUE;
		if ( !m_bTimer && !m_rcClient.PtInRect(point))
		{
			// mouse went outside the window, so we need to scroll even if mouse doesn't move
			SetTimer ( ID_TIMER, 200, 0 );
			m_bTimer = TRUE;
		}
		else if ( m_bTimer && m_rcClient.PtInRect(point))
		{
			// mouse is inside window, so we don't need the timer
			KillTimer ( ID_TIMER );
			m_bTimer = FALSE;
		}
		m_prevMousePoint = point;
		RecalcSelection(point);
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CHexEdit::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/) 
{
	int nPrevViewPos = m_viewPos;

	switch(nSBCode)
	{
	case SB_LINEDOWN:
		m_viewPos += m_columns;
		break;
	case SB_LINEUP:
		m_viewPos -= m_columns;
		break;
	case SB_PAGEDOWN:
		m_viewPos += m_columns*m_charCountWindow.cy;
		break;
	case SB_PAGEUP:
		m_viewPos -= m_columns*m_charCountWindow.cy;
		break;
	case SB_THUMBTRACK:
		SCROLLINFO si;

		si.cbSize = sizeof(si);
		GetScrollInfo ( SB_VERT, &si, SIF_TRACKPOS );
		m_viewPos = si.nTrackPos*m_columns;
		break;
	}

	if ( m_viewPos < 0 )
		m_viewPos = 0;
	DWORD dwDataLen = m_aBuffer.GetSize()/(m_digitsInData/2)/m_columns * m_columns;
	if ( DWORD(m_viewPos) > dwDataLen - m_columns*(m_charCountWindow.cy-1) )
		m_viewPos = dwDataLen - m_columns*(m_charCountWindow.cy-1);

	if ( m_viewPos != nPrevViewPos )
	{
		Invalidate();
		SetScrollPos ( SB_VERT, m_viewPos / m_columns );
		PlaceCaret();
	}
}

void CHexEdit::OnEditCut()
{
	if ( (m_editMode & EDIT_INSERT) || m_allowChangeSize )
	{
		if ( !IsSelectionEmpty() ) 
		{
			OnEditCopy();
			DeleteSelection();
			Invalidate();
		}
	}
}

void CHexEdit::OnEditCopy()
{
	if ( IsSelectionEmpty() ) return;

	if ( !OpenClipboard() )
		return;
	VERIFY(::EmptyClipboard());

	// size of selection in bytes
	int	dwLen = (m_selEnd-m_selStart+1)*(m_digitsInData/2);

	// alloc memory for "BinaryData" clipboard format
	HGLOBAL	hMemb = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT, dwLen+sizeof(DWORD));
	// alloc memory for CF_TEXT clipboard format
	HGLOBAL	hMema = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT, dwLen+1);
	
	ASSERT ( hMemb );
	ASSERT ( hMema );
	
	// copy binary
	BYTE* buf;
	LPBYTE	p = (BYTE*)::GlobalLock(hMemb);

	// first DWORD in BinaryData contains the length of data in bytes
	*(DWORD*) p = dwLen;
	p += sizeof(DWORD);

    buf = m_aBuffer.GetData();
	memcpy(p, buf+m_selStart*(m_digitsInData/2), dwLen);

	::GlobalUnlock(hMemb);
	
	// copy text
	p = (BYTE*)::GlobalLock(hMema);
	memcpy(p, buf+m_selStart*(m_digitsInData/2), dwLen);
	p[dwLen] = 0;

	::GlobalUnlock(hMema);

	VERIFY(::SetClipboardData((USHORT)RegisterClipboardFormat(_T("BinaryData")), hMemb));
	VERIFY(::SetClipboardData(CF_TEXT, hMema));
	::CloseClipboard();
}

void CHexEdit::OnEditPaste()
{
	BOOL bIsBinaryData = FALSE;
	HGLOBAL hmem = NULL;
	UINT hFormat = RegisterClipboardFormat(_T("BinaryData"));
	// we can paste CF_TEXT or BinaryData
	if (!::IsClipboardFormatAvailable((USHORT)hFormat) && !::IsClipboardFormatAvailable(CF_TEXT))
		return;
	
	VERIFY ( OpenClipboard() );
	if (::IsClipboardFormatAvailable((USHORT)hFormat))
	{
		hmem = ::GetClipboardData((USHORT)hFormat);
		bIsBinaryData = TRUE;
	}
	else if (::IsClipboardFormatAvailable(CF_TEXT))
	{
		hmem = ::GetClipboardData(CF_TEXT);
	}
	
	if(hmem)
	{
		LPBYTE	p = (BYTE*)::GlobalLock(hmem);
		DWORD	dwLen;
		if ( bIsBinaryData )
		{
			// first DWORD in BinaryData contains the length of data in bytes
			dwLen = *(DWORD*)p;
			p += sizeof(DWORD);
		}
		else
			dwLen = ::GlobalSize(hmem)-1;
		
		if ( dwLen > 0 )
		{
			if ( m_editMode & EDIT_INSERT )
			{
				DeleteSelection();
				InsertData ( m_editPos, (dwLen+m_digitsInData/2-1)/(m_digitsInData/2) );
			}
			else 
			{
				if ( !IsSelectionEmpty() )
				{
					m_editPos = m_selStart;
					if ( dwLen > ULONG(m_selEnd - m_selStart + 1) )
						dwLen = m_selEnd - m_selStart + 1;
					ClearSelection();
				}
				
				DWORD dwDataLen = m_aBuffer.GetSize();
				if ( m_editPos*(m_digitsInData/2)+dwLen > dwDataLen )
				{
					// need to add bytes at the end
					if (m_allowChangeSize)
						InsertData ( dwDataLen/(m_digitsInData/2), (dwLen - (dwDataLen-m_editPos*(m_digitsInData/2))+(m_digitsInData/2)-1)/(m_digitsInData/2) );
					else
						dwLen = dwDataLen - m_editPos*(m_digitsInData/2);
				}
			}
			
			if ( dwLen > 0 )
			{
				BYTE* buf = m_aBuffer.GetData();
				memcpy ( buf+m_editPos*(m_digitsInData/2), p, dwLen );
				
				m_dataModified = TRUE;
				Invalidate();
			}
		}
		::GlobalUnlock(hmem);
	}
	::CloseClipboard();
}

void CHexEdit::OnTimer(UINT nIDEvent) 
{
	// called when mouse went outside window during selection
	ASSERT ( m_bTimer );

	if ( nIDEvent == ID_TIMER )
	{
		RecalcSelection ( m_prevMousePoint );
	}
	else
		CWnd::OnTimer(nIDEvent);
}

void CHexEdit::OnContextMenu(CWnd*, CPoint point)
{
	UNREFERENCED_PARAMETER(point);
    return;

/*
	if ( LPCVARIANT(m_data)->vt != (VT_ARRAY | VT_UI1) )
		return;
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);
			
			point = rect.TopLeft();
			point.Offset(5, 5);
		}
		
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_HEX_EDIT_CTRL));
		
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
*/
}


void CHexEdit::OnEditSelectAll()
{
	if ( m_aBuffer.GetSize() == 0 ) 
		return;

	m_editPos = m_selStart = 0;
	m_selEnd = m_aBuffer.GetSize()/(m_digitsInData/2)-1;
	ChangeEditPos (0, 0, TRUE );	// place caret & repaint
}

void CHexEdit::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!IsSelectionEmpty());
}

void CHexEdit::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!IsSelectionEmpty() && ((m_editMode & EDIT_INSERT) || m_allowChangeSize));
}

void CHexEdit::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	UNREFERENCED_PARAMETER(pCmdUI);
    return;

/*
    COleDataObject	obj;
	if (obj.AttachClipboard())
		pCmdUI->Enable(obj.IsDataAvailable(CF_TEXT) || obj.IsDataAvailable((USHORT)RegisterClipboardFormat(_T("BinaryData"))));
	else
		pCmdUI->Enable(FALSE);
*/
}

void CHexEdit::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/) 
{
	int nPrevHorzScroll = m_nHorzScroll;
	switch(nSBCode)
	{
	case SB_LINELEFT:
		m_nHorzScroll--;
		break;
	case SB_PAGELEFT:
		m_nHorzScroll -= m_charCountWindow.cx;
		break;
	case SB_LINERIGHT:
		m_nHorzScroll++;
		break;
	case SB_PAGERIGHT:
		m_nHorzScroll += m_charCountWindow.cx;
		break;
	case SB_THUMBTRACK:
		SCROLLINFO si;

		si.cbSize = sizeof(si);
		GetScrollInfo ( SB_HORZ, &si, SIF_TRACKPOS );
		m_nHorzScroll = si.nTrackPos;
		break;
	}
	if ( m_nHorzScroll < 0 )
		m_nHorzScroll = 0;
	else if ( m_nHorzScroll > m_charCountView.cx - m_charCountWindow.cx )
		m_nHorzScroll = m_charCountView.cx - m_charCountWindow.cx;

	if ( nPrevHorzScroll != m_nHorzScroll )
	{
		Invalidate();
		SetScrollPos ( SB_HORZ, m_nHorzScroll );
		PlaceCaret();
	}
}

void CHexEdit::OnPaint() 
{
	CPaintDC PaintDC(this); // device context for painting
    CDC* pDC = &PaintDC;
	
    CRect rcInvalid;
    GetClientRect(rcInvalid);

    if ( rcInvalid.IsRectEmpty() )
		return;

    if( m_aBuffer.GetSize() == 0 )
	{
	    // Paint the background using the BackColor property when no data set
        CBrush bkBrush(m_clrBack);
	    pDC->FillRect(rcInvalid, &bkBrush);
		return;
	}

	CRect rc;
	GetClientRect(rc);

	CDC	dc;
	dc.CreateCompatibleDC(pDC);

	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	dc.SelectObject(bm);

	// paint half-letter-wide rect at the left of the text
	CRect rc1 ( 0, rcInvalid.top, m_cellSize.cx/2, rcInvalid.bottom );
	CBrush bkBrush(m_clrBack);
    dc.FillRect(rc1, &bkBrush);

	CFont font;
	font.CreatePointFont ( m_fontHeight * 10, _T("Courier") );
	CFont* oldFont = dc.SelectObject ( &font );

	dc.SetBoundsRect(&rc, DCB_DISABLE);

    CString strText;
	int nAddr = m_viewPos*(m_digitsInData/2);
	BYTE* buf = m_aBuffer.GetData();

	for ( int i = 0; i < m_charCountWindow.cy; i++ )
		// draw line
	{
		if ( m_showAddress )
		{
			// draw address field
			if (nAddr <= m_aBuffer.GetSize() ) ///+ m_columns*(m_digitsInData/2)/)
				strText.Format ( _T("%0*X  "), m_digitsInAddress, nAddr+m_dwStartAddr );
			else
				strText.Format ( _T("%*c  "), m_digitsInAddress, ' ' );
			dc.SetTextColor(m_clrFont);
			dc.SetBkColor(m_clrBack);
			TextOutWithCheck ( &dc, rcInvalid, -m_nHorzScroll*m_cellSize.cx+m_cellSize.cx/2, i*m_cellSize.cy, strText );
		}
		for ( int j = 0; j < m_columns; j++ )
		{
			// draw hex data
			int dwPos = j+i*m_columns+m_viewPos;
			if ( dwPos >= m_selStart && dwPos <= m_selEnd)
			{
				// selected text colors
				dc.SetBkColor(::GetSysColor ( COLOR_HIGHLIGHT ));
				dc.SetTextColor(::GetSysColor ( COLOR_HIGHLIGHTTEXT ));
			}
			else
			{
				// normal text colors
				dc.SetTextColor(m_clrFont);
				dc.SetBkColor(m_clrBack);
			}
			
			if ( dwPos < m_aBuffer.GetSize()/(m_digitsInData/2))
			{
				switch (m_digitsInData)
				{
				case 2:
					strText.Format ( _T("%02X "), buf[dwPos]);
					break;
				case 4:
                    
                    if ( m_bBigEndian ) // Big Endian
                    {
                        strText.Format ( _T("%02X%02X "), buf[dwPos * 2], buf[dwPos * 2 + 1]);
                    }
                    else // Little Endian
                    {
                        strText.Format ( _T("%04X "), *(WORD*)(buf + dwPos * 2) );
                    }
                    break;
                case 8:
                    if ( m_bBigEndian ) // Big Endian
                    {
                        strText.Format ( _T("%02X%02X%02X%02X "), buf[dwPos * 4],
                                                                  buf[dwPos * 4 + 1],
                                                                  buf[dwPos * 4 + 2],
                                                                  buf[dwPos * 4 + 3] );
                    }
                    else // Little Endian
                    {
                        strText.Format ( _T("%08X "), *(DWORD*)(buf + dwPos * 4) );
                    }
                    
                    break;
				default:
					ASSERT(0);
				}

				TextOutWithCheck ( &dc, rcInvalid, (m_addrMargin+j*(m_digitsInData+1)-m_nHorzScroll)*m_cellSize.cx+m_cellSize.cx/2, 
					i*m_cellSize.cy, strText );
				if ( m_bRealShowAscii )
				{
					// draw ascii data
					ASSERT ( m_digitsInData == 2 );
					strText.Format(_T("%d"), isprint ( buf[dwPos] ) ? buf[dwPos] : '.');
					TextOutWithCheck ( &dc, rcInvalid, (m_asciiMargin+j-m_nHorzScroll)*m_cellSize.cx+m_cellSize.cx/2, i*m_cellSize.cy, strText );
				}
			}
			else // if ( DWORD(dwPos)
			{
				// if the last line has less than m_columns numbers, fill it with spaces
				dc.SetTextColor(m_clrFont);
				dc.SetBkColor(m_clrBack);
				strText.Format ( _T("%*c "), m_digitsInData, ' ' );
				TextOutWithCheck ( &dc, rcInvalid, (m_addrMargin+j*(m_digitsInData+1)-m_nHorzScroll)*m_cellSize.cx+m_cellSize.cx/2, i*m_cellSize.cy, strText );
				if ( m_bRealShowAscii )
				{
					ASSERT ( m_digitsInData == 2 );
					strText = ' ';
					TextOutWithCheck ( &dc, rcInvalid, (m_asciiMargin+j-m_nHorzScroll)*m_cellSize.cx+m_cellSize.cx/2, i*m_cellSize.cy, strText );
				}
			}
		}

		dc.SetTextColor(m_clrFont);
		dc.SetBkColor(m_clrBack);

		if ( m_bRealShowAscii )
		{
			// draw space between hex and ascii
			strText = ' ';
			TextOutWithCheck ( &dc, rcInvalid, (m_asciiMargin-m_nHorzScroll-1)*m_cellSize.cx+m_cellSize.cx/2, i*m_cellSize.cy, strText );
		}		

		// draw extra spaces at the end of line
		dc.SetTextColor(m_clrFont);
		dc.SetBkColor(m_clrBack);
		if ( m_charCountView.cx < m_charCountWindow.cx )
			strText.Format(_T("%*c  "), m_charCountWindow.cx-m_charCountView.cx+1, ' ' );
		else
			strText = _T("  ");
		TextOutWithCheck ( &dc, rcInvalid, (m_asciiMargin + (m_bRealShowAscii?m_columns:0) -m_nHorzScroll)*m_cellSize.cx+m_cellSize.cx/2, i*m_cellSize.cy, strText );
		nAddr += m_columns*(m_digitsInData/2);
	}

	// fill the rest of the window with spaces
	strText.Format ( _T("%*c "), m_charCountWindow.cx, ' ' );
	TextOutWithCheck ( &dc, rcInvalid, m_cellSize.cx/2, m_charCountWindow.cy*m_cellSize.cy, strText );
	dc.SelectObject ( oldFont );

    pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);	
	// Do not call CWnd::OnPaint() for painting messages
}