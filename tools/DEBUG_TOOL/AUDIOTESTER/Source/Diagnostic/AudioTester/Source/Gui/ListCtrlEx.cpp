// ListCtrlEx.cpp : implementation file

/********************************************************************
**  filename: 	 ListCtrlEx.cpp 
**	author:		Xue Wei	
**	Date:	8/4/2004   16:08
**	Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.       

**	Description: a list control which enhanced the MFC's standard list control , by
               using this ,you can edit and spin every cell of list
*********************************************************************
*********************************************************************
Edit History
**	DATE                   NAME             DESCRIPTION      
**	8/4/2004              Xue Wei              Create.        

*********************************************************************/


#include "stdafx.h"
#include "AudioTester.h"
#include "ListCtrlEx.h"
#include "InPlaceEdit.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define IDC_IPEDIT 678
#define IDC_COMBOX 681
/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx()
{
    m_nCurrentSelectionRow =-1;
    m_nCurrentSelectionCol = -1;
    m_nSpinLow = 1;
    m_nSpinHigh = 100;    
    m_enShowType = SPIN_EDIT_TYPE;
    m_iFloatPrecision = 0;
    m_enumEditType = CInPlaceEdit::DEFAULT;
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT_EX(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

// HitTestEx - Determine the row index and column index for a point
// Returns   - the row index or -1 if point is not over a row
// point     - point to be tested.
// col       - to hold the column index
int CListCtrlEx::HitTestEx(CPoint& point, int* pCol)
{
	int nColnum = 0;
	int nRow = HitTest(point, NULL);

   if(pCol) 
      *pCol = 0;

	// Make sure that the ListView is in LVS_REPORT
	if((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return nRow;

   // Get the number of columns
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();

	// Get the top and bottom row visible
	nRow = GetTopIndex();
	
   int bottom = nRow + GetCountPerPage();
   if(bottom > GetItemCount())
		bottom = GetItemCount();

	// Loop through the visible rows
	for( ;nRow <=bottom; nRow++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		if( GetItemRect(nRow, &rect, LVIR_BOUNDS) )
		{
			if( rect.PtInRect(point) )
			{
				// Now find the column
				for( nColnum = 0; nColnum < nColumnCount; nColnum++ )
				{
					int colwidth = GetColumnWidth(nColnum);
					if(point.x >= rect.left && point.x <= (rect.left + colwidth))
					{
						if(pCol) 
							*pCol = nColnum;
						return nRow;
					}
					rect.left += colwidth;
				}
			}
		}
		else if( pCol )
		{
			// Is pre item is empty
			if (nRow > 0 && IsPreItemEmpty(nRow) )
				return -1;
			if(IsItemEmpty(nRow))
				return -1;
			
			// Clicked on the 'empty' row
			int newInsertIndex = InsertItem( nRow + 1, _T("") );
			SetItemState( newInsertIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
			GetItemRect(nRow, &rect, LVIR_BOUNDS);
			if(rect.PtInRect(point))
			{
				// Now find the column
				for( nColnum = 0; nColnum < nColumnCount; nColnum++ )
				{
					int colwidth = GetColumnWidth(nColnum);
					if(point.x >= rect.left && point.x <= (rect.left + colwidth))
					{
						*pCol = nColnum;
						return nRow;
					}
					rect.left += colwidth;
				}
			}
			else
			{
				DeleteItemEx( newInsertIndex );
			}
		}
	}
	return -1;
}

void CListCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(GetFocus() != this ) 
      SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(GetFocus() != this) 
      SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CListCtrlEx::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* plvDispInfo = (LV_DISPINFO *)pNMHDR;
 	LV_ITEM* plvItem = &plvDispInfo->item;
   GetItemText(plvItem->iItem, plvItem->iSubItem);

   if(plvItem->pszText != NULL)
	   SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
   *pResult = FALSE;
   return FALSE;
}


// EditLabel		- Start edit of a sub item label
// Returns		- Temporary pointer to the new edit control
// nItem		- The row index of the item to edit
// nCol			- The column of the sub item.

CEdit* CListCtrlEx::EditLabel(int nItem, int nCol)
{
	// Make sure that the item is visible
	if(!EnsureVisible(nItem, TRUE)) 
      return NULL;

	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();
	if(nCol >= nColumnCount || GetColumnWidth(nCol) < 5)
		return NULL;

	// Get the column offset
	int offset = 0;
	for(int i = 0; i < nCol; i++)
		offset += GetColumnWidth(i);

    CRect rect;
	GetItemRect(nItem, &rect, LVIR_BOUNDS);

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(&rcClient);
	if(offset + rect.left < 0 || offset + rect.left > rcClient.right)
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}
	// Get Column alignment
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn(nCol, &lvcol);

	rect.left += offset+4;
	rect.right = rect.left + GetColumnWidth(nCol) - 3 ;
	if(rect.right > rcClient.right) 
      rect.right = rcClient.right;
	
    if (!m_editOnly.GetSafeHwnd())
    {        
        DWORD dwEditStyle = WS_CHILD | WS_VISIBLE|WS_BORDER;                      
        m_editOnly.Create(dwEditStyle, rect, this, ID_MYSPIN_EDIT);
        m_editOnly.SetEditType(m_enumEditType);        
    }
    else
    {
        m_editOnly.MoveWindow( &rect );
    }

    m_editOnly.SetWindowText( GetItemText( nItem, nCol ) );
    m_editOnly.ShowWindow(SW_SHOW);
    //m_editOnly.BringWindowToTop();
    m_editOnly.SetFocus();
    m_editOnly.SetSel(0,-1);
	return &m_editOnly;
}



CWnd* CListCtrlEx::EditSpinLabel(int nItem, int nCol)
{
	if(!EnsureVisible(nItem, TRUE)) 
      return NULL;
	
	// Set title and percent
	CString strPercent;
	strPercent = GetItemText( nItem, nCol );	
    strPercent.Replace( m_strSpinTitle ,_T(""));
	m_SpinEdit.SetTitle( m_strSpinTitle );

	// Create or move it
	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();
	if(nCol >= nColumnCount || GetColumnWidth(nCol) < 5)
		return NULL;

	// Get the column offset
	int offset = 0;
	for(int i = 0; i < nCol; i++)
		offset += GetColumnWidth(i);

	CRect rect;
	GetItemRect(nItem, &rect, LVIR_BOUNDS);

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(&rcClient);
	if(offset + rect.left < 0 || offset + rect.left > rcClient.right)
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}

	rect.left += offset+4;
	rect.right = rect.left + GetColumnWidth(nCol) - 3 ;
	if(rect.right > rcClient.right) 
	rect.right = rcClient.right;
	
	if (!m_SpinEdit.GetSafeHwnd())
		m_SpinEdit.Create(_T("STATIC"), _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER, rect, this, NULL);
	else
		m_SpinEdit.MoveWindow( &rect );
	
//    	
	//	int iPercent = 0;
	//	if (strPercent.GetLength() > 0)
	//	{        
	//		iPercent = atoi(strPercent.GetBuffer(0));
	//	}
	
    m_SpinEdit.SetPosFlag( true );
	m_SpinEdit.SetShowNumber( strPercent);

	m_SpinEdit.ShowAllWindow(SW_SHOW);
	m_SpinEdit.BringWindowToTop();
	//m_SpinEdit.SetFocus();
    m_SpinEdit.SetFocusOnEdit();
	m_SpinEdit.SetSpinRange( m_nSpinLow, m_nSpinHigh );	
    return &m_SpinEdit;
}


int CListCtrlEx::InsertColumn(int nCol, const LV_COLUMN* pColumn)
{
   
   return CListCtrl::InsertColumn(nCol, pColumn);
}


void CListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CListCtrl::OnLButtonDblClk(nFlags, point);	
	int nindex=-1;
	int nColnum=-1;
	if((nindex = HitTestEx(point, &nColnum)) != -1)
	{
        m_nCurrentSelectionRow = nindex;		
        m_nCurrentSelectionCol = nColnum;
	    ::SendMessage( this->GetParent()->GetSafeHwnd(), WM_LISTEX_COLLUM_INDEX,WPARAM ( nindex ),0);	
		if( GetItemData( nindex ) == -1 )
            return;

//		UINT flag = LVIS_FOCUSED;
		
        switch( m_enShowType )
		{
        case NOT_EDIT:
            return;
		case COMBOX_TYPE:
            DropDownComBox( nindex, nColnum );
			break;			
		case SPIN_EDIT_TYPE:
			EditSpinLabel( nindex, nColnum);
			break;
        case ONLY_EDIT_TYPE: 
            EditLabel( nindex, nColnum );
            break;
		default:
            ASSERT(0);
    
		}
		
	}
}

void CListCtrlEx::OnRButtonDown(UINT nFlags, CPoint point) 
{
   SetFocus();
  int nIndex=-1;
  int nColnum=-1;
  if((nIndex = HitTestEx(point, &nColnum)) != -1)
   {
      m_nCurrentSelectionRow = nIndex;
      m_nCurrentSelectionCol = nColnum; 
      SetItemState(nIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
   }
 
	CListCtrl::OnRButtonDown(nFlags, point);
}

void CListCtrlEx::DeleteItemEx(int nIndex)
{
	if( DeleteItem( nIndex ) )
	{
		if( nIndex > 0 )
		{
	      m_nCurrentSelectionRow = nIndex - 1;
		   SetItemState(nIndex-1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
		else if( GetItemCount()>0 )
		{
	      m_nCurrentSelectionRow = 0;
		   SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
		else
		{
			m_nCurrentSelectionRow = -1;
		}
	}
}

LRESULT CListCtrlEx::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_INPLACE_SPIN)
	{
		LPSPINTEXT pst = (LPSPINTEXT) lParam;

		SetItemText( m_nCurrentSelectionRow, m_nCurrentSelectionCol, m_strSpinTitle + pst->m_strShowValue );
		GetParent()->SendMessage( message, wParam, lParam );
		return TRUE;
	}
    else if( message == WM_INPLACE_COMBOX )
    {
        LPCOMBOX_DATA pData = (LPCOMBOX_DATA)lParam;
        ASSERT( pData->m_nSelectIdx >= 0 && pData->m_nSelectIdx < (int)m_vComboxStrings.size() );
        SetItemText( m_nCurrentSelectionRow, m_nCurrentSelectionCol, m_vComboxStrings[ pData->m_nSelectIdx ] );
	    GetParent()->SendMessage( message, wParam, lParam );
		return TRUE;
    }
    else if( message == WM_INPLACE_KILL_FOCUS )
    {
        CInPlaceEdit* pEdit = (CInPlaceEdit*)lParam;
        CString strText;
        pEdit->GetWindowText(strText);
        if( CInPlaceEdit::FLOAT_NUMBER == pEdit->GetEditType() )
        {
			double dValue = 0.0;
#ifdef _UNICODE
			char szText[32] = {0};
			WideCharToMultiByte(CP_ACP,0,strText,-1,szText,16,NULL,NULL);
			dValue = atof( szText );
#else
			dValue = atof( (LPSTR)(LPCTSTR)strText );
#endif
            CString strFormat;
            strFormat.Format(_T("%%.%df"),m_iFloatPrecision);
            strText.Format(strFormat,dValue );
        }

		GetParent()->SendMessage( message, wParam, (LPARAM)&strText );
            
        SetItemText( m_nCurrentSelectionRow, m_nCurrentSelectionCol, strText ); 
        return TRUE;
    }
	return CListCtrl::DefWindowProc(message, wParam, lParam);
}
BOOL CListCtrlEx::IsPreItemEmpty(int nItem)
{
	ASSERT(nItem >= 0);
	if (nItem < 1 || nItem > GetItemCount())
		return FALSE;
	
	int nPre = nItem - 1;
	
	BOOL bEmpty = TRUE;
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();
	for (int col = 0; col < nColumnCount; col++)
	{
		CString str = GetItemText(nPre, col);
		str.TrimLeft(); str.TrimRight();
		if (!str.IsEmpty())
		{
			bEmpty = FALSE;
			break;
		}
	}

	return bEmpty;
}

BOOL CListCtrlEx::IsItemEmpty(int nItem)
{
	ASSERT(nItem >= 0);
	if ( nItem >= GetItemCount())
		return TRUE;
	
	int nPre = nItem ;
	
	BOOL bEmpty = TRUE;
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();
	for (int col = 0; col < nColumnCount; col++)
	{
		CString str = GetItemText(nPre, col);
		str.TrimLeft(); str.TrimRight();
		if (!str.IsEmpty())
		{
			bEmpty = FALSE;
			break;
		}
	}

	return bEmpty;
}

void CListCtrlEx::DropDownComBox(int _nRow, int _nColnum )
{
    if(!EnsureVisible(_nRow, TRUE)) 
      return ;

	// Make sure that nCol is valid 
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();
	if(_nColnum >= nColumnCount || GetColumnWidth(_nColnum) < 10)
		return ;

	// Get the column offset
	int nOffset = 0;
	for(int i = 0; i < _nColnum; i++)
		nOffset += GetColumnWidth(i);

	CRect rect;
	GetItemRect(_nRow, &rect, LVIR_BOUNDS);

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(&rcClient);
	if(nOffset + rect.left < 0 || nOffset + rect.left > rcClient.right)
	{
		CSize size;
		size.cx = nOffset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}
	rect.left += (nOffset + 4);
	rect.right = rect.left + GetColumnWidth(_nColnum) - 3;
	int nHeight = rect.bottom - rect.top;
	rect.bottom += (10 * nHeight);
	if(rect.right > rcClient.right) 
      rect.right = rcClient.right;

    if (!m_ComBox.GetSafeHwnd())
    {
        DWORD dwStyle = WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_DISABLENOSCROLL;
        m_ComBox.Create(dwStyle, rect, this, IDC_COMBOX );
        m_ComBox.SetFont( this->GetFont() );
    }	
	else
    {
        m_ComBox.MoveWindow( &rect );
    }
	
    m_ComBox.SetItemHeight(-1, nHeight);
	m_ComBox.SetHorizontalExtent(GetColumnWidth(_nColnum));


	CString csSelStr = GetItemText( _nRow, _nColnum );
    m_ComBox.SetShowStrings( m_vComboxStrings );
    m_ComBox.ShowTime();
    m_ComBox.SelectString(0, csSelStr );
    m_ComBox.ShowWindow( SW_SHOW );
    m_ComBox.SetFocus();
    m_ComBox.ShowDropDown();
	
}

void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{  
    if( m_ComBox.GetSafeHwnd() )
    {
        m_ComBox.ShowWindow( SW_HIDE );
    }   
	
	CListCtrl::OnLButtonDown(nFlags, point);
}

