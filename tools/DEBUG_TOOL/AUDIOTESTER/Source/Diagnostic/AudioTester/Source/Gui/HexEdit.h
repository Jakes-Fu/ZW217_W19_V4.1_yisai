#if !defined(AFX_HEXEDIT_H__D57C36A9_11B0_4FD8_A589_CE5AE68B4F6D__INCLUDED_)
#define AFX_HEXEDIT_H__D57C36A9_11B0_4FD8_A589_CE5AE68B4F6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEdit.h : header file
//

#include <Afxtempl.h>

// Notify the parent window that the buffer data has been changed by user
#define WM_HE_UNITDATA_CHANGED  WM_APP + 1001

/////////////////////////////////////////////////////////////////////////////
// CHexEdit window

class CHexEdit : public CWnd
{
// Constructor and destructor
public:
	CHexEdit();
	virtual ~CHexEdit();

// Public interface
public:
    void  SetData(void* pData,int nSize,unsigned int dwStartAddr = 0);
    void* GetData(int& nSize);

    short GetDigitsInAddress();
    void  SetDigitsInAddress(short nNewValue);
    short GetDigitsInData();
    void  SetDigitsInData(short nNewValue);
    short GetColumns();
    void  SetColumns(short nNewValue); // 0 indicates auto-column
    short GetFontHeight();
    void  SetFontHeight(short nNewValue);
    void  GetColor( COLORREF &clrBack, COLORREF &clrFont );
    void  SetColor( COLORREF clrBack, COLORREF clrFont );
    BOOL  GetEndianMode();
    void  SetEndianMode(BOOL bBigEndian);
    
    void  ShowAddress(BOOL bShow);
    void  ShowAscii(BOOL bShow);
    void  EnableChangingDataSize(BOOL bEnable);

// Attributes
protected:
    /*! Data buffer */
    CByteArray m_aBuffer;

    /*! Background color & font color */
    COLORREF m_clrBack;
    COLORREF m_clrFont;

    /*! TRUE if using little endian */
    BOOL m_bBigEndian;

	/*! Number of hex digits shown in address field (4 or 8).*/
	short m_digitsInAddress;

	/*! Number of hex digits in one number
	 - 2 - BYTE view
  	 - 4 - WORD view
	 - 8 - DWORD view
	 */
	short m_digitsInData;

	/*! Number of columns.*/
	short m_columns;

	/*! Font height.*/
	short m_fontHeight;

	/*! Starting address to display.*/
	DWORD m_dwStartAddr;

	/*! True if data are modified.*/
	BOOL m_bDataModified;
    
// Other data

	/*! This value equals the following expression: (m_showAscii && m_digitsInData == 2).*/
	BOOL m_bRealShowAscii;

	/*! Current horizontal scroll shift in chars.*/
	int m_nHorzScroll;

	/*! True after WM_LBUTTONDOWN, FALSE after WM_LBUTTONUP.*/
	BOOL m_bMouseDown;

	/*! True after WM_MOUSEMOVE, FALSE after WM_LBUTTONUP.*/
	BOOL m_bMouseMove;

	/*! Address of number under mouse cursor in WM_LBUTTONDOWN.*/
	int m_posMouseDown;

	/*! Window size in chars.*/
	CSize m_charCountWindow;

	/*! Total view size in chars.*/
	CSize m_charCountView;

	/*! Char size in device points.*/
	CSize m_cellSize;

	/* !!! IMPORTANT
	m_selStart, m_selEnd and other addresses of numbers are counts of numbers (NOT NESESSARY BYTES!),
	depending on m_digitsInData.*/

	/*! Selection start (in numbers).*/
	int m_selStart;

	/*! Selection end (in numbers).*/
	int m_selEnd;

	/*! Address of number being edited.*/
	int m_editPos;

	/*! Address of the first visible number.*/
	int m_viewPos;

	/*! Digit being edited (0 - leftmost).*/
	int m_nEditDigit;

	/*! Right margin of address field in chars.*/
	int m_addrMargin;

	/*! Left margin of ASCII field in chars.*/
	int m_asciiMargin;

	/*! This value is a combination of following flags:
	- EDIT_BYTES - edit hex data mode (if not set - edit ASCII data)
	- EDIT_INSERT - insert mode (if not set - overwrite mode)
	*/
	int m_editMode;

	/*! Control client rect.*/
	CRect m_rcClient;

	/*! True if timer is active.*/
	BOOL m_bTimer;

	/*! Coordinates of last point in OnMouseMove.*/
	CPoint m_prevMousePoint;

	/*! This function calls pdc->TextOut ( x, y, strText ) function if text overlaps with rcInvalid.*/
	void TextOutWithCheck(CDC* pdc, const CRect& rcInvalid, int x, int y, CString& strText );

	/*! This function called before entering a new number or ascii byte.
	 * It does InsertBytes, scrolling, repainting if nesessary.
	 * @return FALSE, if it's impossible to insert number because of
	 * m_allowChangeSize == FALSE, otherwise - TRUE.
	 */
	BOOL EnterNumber();

	/*! Clear selection.*/
	void ClearSelection();

	/*! Delete currently selected data.*/
	void DeleteSelection();

	/*! Update selection margins when mouse is moving.
	 * @param point mouse coordinates.
	 */
	void RecalcSelection(CPoint point);

	/*! Returns TRUE, if nothing is selected, and FALSE otherwise.*/
	BOOL IsSelectionEmpty();

	/*! Recalculate scroll ranges and positions of scrollbars,
	 * show or hide scroll bars if nesessary.
	 */
	BOOL UpdateScrollBars();

	/*! Invalidate rect occupied by number with address nEditPos.*/
	void InvalidateEditRect(int nEditPos);

	/*! Delete dwCount numbers (not bytes!) from m_data, starting from dwPos.*/
	void DeleteData ( DWORD dwPos, DWORD dwCount = 1 );

	/*! Insert dwCount numbers (not bytes!) in m_data, starting from dwPos.*/
	void InsertData ( DWORD dwPos, DWORD dwCount = 1 );

	/*! Update caret position and scroll bars when m_editPos is changed.*/
	BOOL NormalizeEditPos();

	/*! Set new edit position after mouse click.
	 * @param pt - mouse coordinates.
	 */
	void ChangeEditPos ( CPoint pt );

	/*! Set new edit position after moving caret.
	 * @param dx - horizontal shift of cursor (in numbers), positive - left, negative - right direction
	 * @param dx - vertical shift of cursor (in numbers), positive - down, negative - upward direction
	 * @param bRepaint = TRUE - repaint the control always,	FALSE - only if scrolling is nesessary
	 */
	void ChangeEditPos ( int dx, int dy, BOOL bRepaint = FALSE);

	/*! Calculate coordinates and show caret.*/
	void PlaceCaret();

	/*! Compute m_editPos, m_viewPos, margins etc. after changing properties of the control.*/
	void RecalcLayout();

// Display format processing

    BOOL m_showAddress;
    BOOL m_showAscii;
    BOOL m_allowChangeSize;
    BOOL m_dataModified;

    void OnShowAddressChanged();
    void OnShowAsciiChanged();
    
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CHexEdit)
	afx_msg void OnPaint();
   	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEditSelectAll();
	afx_msg void OnContextMenu(CWnd*, CPoint point);	// right click handler
	afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
    int ResizeBuf(int lNewSize);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXEDIT_H__D57C36A9_11B0_4FD8_A589_CE5AE68B4F6D__INCLUDED_)
