// ListEditCtrl.cpp : implementation file
//

// The code contained in this file is based on the original
// CInPlaceEdit from http://www.codeguru.com/listview/edit_subitems.shtml

#include "stdafx.h"
#include "NVEditor.h"
#include "ListEditCtrl.h"
#include "NVItemDef.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_CHAR_COUNT (10)
/////////////////////////////////////////////////////////////////////////////
// CListEditCtrl
BEGIN_MESSAGE_MAP(CListEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CListEditCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEditCtrl message handlers

CListEditCtrl::CListEditCtrl(int iItem, int iSubItem, CString sInitText):m_strInitText(sInitText)
{	
	m_iItem = iItem;
	m_iSubItem = iSubItem;
	m_iItemType = 0;
    m_iItemLength = 0;	
	m_bVK_ESCAPE = 0;
    m_iCharCount = m_strInitText.GetLength();
    m_LastChar = 0;
    m_bHex = FALSE;
}

int CListEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)		
		return -1;	

	CFont* font = GetParent()->GetFont();
	SetFont(font);
	m_strInitText.MakeUpper ();
    if( (0 <= m_strInitText.Find('x')) ||
        (0 <= m_strInitText.Find('X')) )
    {
       m_bHex =TRUE;
    }
    SetWindowText(m_strInitText);
	SetFocus();	
	SetSel(0, -1);
	return 0;
}

BOOL CListEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )	
	{		
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_DELETE || pMsg->wParam == VK_ESCAPE || GetKeyState( VK_CONTROL))
		{			
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return 1;
		}	
	}
	return CEdit::PreTranslateMessage(pMsg);
}


void CListEditCtrl::OnKillFocus(CWnd* pNewWnd)
{	
	CEdit::OnKillFocus(pNewWnd);

	CString str;	GetWindowText(str);

	str.MakeUpper();
	if(str.Find(_T("0X")) != -1)
	{
		m_bHex = TRUE;
	}
	else
	{
		m_bHex = FALSE;
	}
    
    unsigned int value = 0;
    if(m_bHex)
    {
        _stscanf((LPCTSTR)str, _T("%X"), &value);
    }
    else
    {
        _stscanf((LPCTSTR)str, _T("%u"), &value);
    }

	if(value != 0)
	{
		//  (unsigned int)atoi(str);
		switch(m_iItemType)
		{
		case CHAR_ITEM_TYPE:        
			if(0xFFFFFF00 & value)
			{            
				str = m_strInitText;
			}        
			break;
		case SHORT_ITEM_TYPE:
			if(0xFFFF0000 & value)
			{            
				str = m_strInitText;
			}  
			break; 
		default:
			break;
		}
	}
	else
	{
		CString strTmp = str;
		if(m_bHex)
		{
			strTmp.MakeUpper();
			strTmp.Delete(0,2);
			strTmp.Remove(_T('0'));
			if(strTmp.GetLength()!= 0)
			{
				str = m_strInitText;
			}
			else
			{
				str = _T("0x0");
			}
		}
		else
		{		
			strTmp.Remove(_T('0'));
			if(strTmp.GetLength()!= 0)
			{
				str = m_strInitText;
			}
			else
			{
				str = _T("0");
			}
		}
	
	}

	// Send Notification to parent of ListView ctrl	
	LV_DISPINFO lvDispInfo;
	lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispInfo.hdr.idFrom = GetDlgCtrlID();	
	lvDispInfo.hdr.code = LVN_ENDLABELEDIT;
	lvDispInfo.item.mask = LVIF_TEXT;	
	lvDispInfo.item.iItem = m_iItem;
	lvDispInfo.item.iSubItem = m_iSubItem;
	lvDispInfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
	lvDispInfo.item.cchTextMax = str.GetLength();
	GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(),(LPARAM)&lvDispInfo);
	DestroyWindow();
}


void CListEditCtrl::OnNcDestroy()
{
	CEdit::OnNcDestroy();	
	delete this;
}


void CListEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)	
	{		
		if( nChar == VK_ESCAPE)
			m_bVK_ESCAPE = 1;		
		GetParent()->SetFocus();
		return;	
	}
    
    
    if(m_iCharCount >=  MAX_CHAR_COUNT)      
    {
        if(!((VK_RETURN == nChar) || (VK_BACK == nChar)))           
           return;
    }


    if(VK_BACK == nChar)
    {
       m_iCharCount --;
       if(0 > m_iCharCount)
       {
          m_iCharCount = 0;
          
       }
       
       if(1 >= m_iCharCount)
       {
          m_bHex = FALSE;
       }
    }
    else if( (nChar >= '0' && nChar <= '9') )
    {
       m_LastChar = (char)nChar;
       m_iCharCount++;
    }
    else if(('x' == nChar) || ('X' == nChar))
    {
        CString strText;
        GetWindowText( strText );
        // if((1 == m_iCharCount) && ('0' == m_LastChar))
        if ( ( 1 == strText.GetLength() ) && ( '0' == strText[0] ) )
        {            
            m_iCharCount++;
            m_bHex = TRUE;
        }
        else
        {
            return;
        }
    }
    else if( (nChar >= 'A' && nChar <= 'F') || 
             (nChar >= 'a' && nChar <= 'f') )
    {
        if(TRUE == m_bHex)
        {
           m_iCharCount++;
        }
        else
        {
            return;
        }
    }
    else if(VK_RETURN != nChar)
    {
       return;
    }
    
    CEdit::OnChar(nChar, nRepCnt, nFlags);	
    
	// Resize edit control if needed
	// Get text extent	
	CString str;	
	GetWindowText( str );
	m_iCharCount = str.GetLength();
	CWindowDC dc(this);
	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = dc.SelectObject(pFont);
	CSize size = dc.GetTextExtent(str);	
	dc.SelectObject(pFontDC);
	size.cx += 5; // add some extra buffer	
	// Get client rect
	CRect rect, rcParent;	
	GetClientRect(&rect);
	GetParent()->GetClientRect(&rcParent);
	// Transform rect to parent coordinates	
	ClientToScreen(&rect);
	GetParent()->ScreenToClient(&rect);
	// Check whether control needs to be resized
	// and whether there is space to grow	
	if(size.cx > rect.Width())	
	{
		if(size.cx + rect.left < rcParent.right)
			rect.right = rect.left + size.cx;		
		else			
			rect.right = rcParent.right;
		MoveWindow(&rect);	
	}
}


void CListEditCtrl::SetItemType(int iItemType,int iItemLength/*=-1*/)
{    
    m_iItemType = iItemType;
	if(iItemLength == -1)
	{
		m_iItemLength = iItemType;
	}
	else
	{
		m_iItemLength = iItemLength;
	}
	if(iItemType == WSTRING_ITEM_TYPE)
	{
		m_iItemLength /= 2;
	}
    
}

/*****************************************************************
CListEditCtrlLite implement
******************************************************************/

BEGIN_MESSAGE_MAP(CListEditCtrlLite, CEdit)
	//{{AFX_MSG_MAP(CListEditCtrlLite)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEditCtrlLite message handlers

CListEditCtrlLite::CListEditCtrlLite(int iItem, int iSubItem, CString sInitText):m_strInitText(sInitText)
{	
	m_iItem = iItem;
	m_iSubItem = iSubItem;
    m_iItemLength = 0;
	m_bVK_ESCAPE = 0;
    m_iCharCount = 0;
    m_LastChar = 0;
}

int CListEditCtrlLite::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)		
		return -1;	

	CFont* font = GetParent()->GetFont();
	SetFont(font);
	//m_strInitText.MakeUpper ();
    SetWindowText(m_strInitText);
	SetFocus();	
	SetSel(0, -1);
	return 0;
}

BOOL CListEditCtrlLite::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )	
	{		
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_DELETE || pMsg->wParam == VK_ESCAPE || GetKeyState( VK_CONTROL))
		{			
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return 1;
		}	
	}
	return CEdit::PreTranslateMessage(pMsg);
}


void CListEditCtrlLite::OnKillFocus(CWnd* pNewWnd)
{	
	CEdit::OnKillFocus(pNewWnd);

	CString str;	GetWindowText(str);
    
	// Send Notification to parent of ListView ctrl	
	LV_DISPINFO lvDispInfo;
	lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispInfo.hdr.idFrom = GetDlgCtrlID();	
	lvDispInfo.hdr.code = LVN_ENDLABELEDIT;
	lvDispInfo.item.mask = LVIF_TEXT;	
	lvDispInfo.item.iItem = m_iItem;
	lvDispInfo.item.iSubItem = m_iSubItem;
	lvDispInfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
	lvDispInfo.item.cchTextMax = str.GetLength();
	GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(),(LPARAM)&lvDispInfo);
	DestroyWindow();
}


void CListEditCtrlLite::OnNcDestroy()
{
	CEdit::OnNcDestroy();	
	delete this;
}


void CListEditCtrlLite::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	USES_CONVERSION;
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)	
	{		
		if( nChar == VK_ESCAPE)
			m_bVK_ESCAPE = 1;		
		GetParent()->SetFocus();
		return;	
	}

	if((1 == m_iSubItem) &&(ASTRING_ITEM_TYPE == m_iItemType ||
		WSTRING_ITEM_TYPE == m_iItemType))
	{
		if(m_iCharCount >=  m_iItemLength)      
		{
			if(!((VK_RETURN == nChar) || (VK_BACK == nChar)))           
			   return;
		}
	}
    
    
    if(VK_BACK == nChar)
    {
       m_iCharCount --;
       if(0 > m_iCharCount)
       {
          m_iCharCount = 0;
          
       }       
    }
   
    CEdit::OnChar(nChar, nRepCnt, nFlags);	
    
	// Resize edit control if needed
	// Get text extent	
	CString str;	
	GetWindowText( str );
	

	if( ASTRING_ITEM_TYPE == m_iItemType)
	{
		m_iCharCount = strlen(W2A(str.operator LPCTSTR()));
	}
	else
	{
		m_iCharCount = str.GetLength();
	}
	
	
	CWindowDC dc(this);
	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = dc.SelectObject(pFont);
	CSize size = dc.GetTextExtent(str);	
	dc.SelectObject(pFontDC);
	size.cx += 5; // add some extra buffer	
	// Get client rect
	CRect rect, rcParent;	
	GetClientRect(&rect);
	GetParent()->GetClientRect(&rcParent);
	// Transform rect to parent coordinates	
	ClientToScreen(&rect);
	GetParent()->ScreenToClient(&rect);
	// Check whether control needs to be resized
	// and whether there is space to grow	
	if(size.cx > rect.Width())	
	{
		if(size.cx + rect.left < rcParent.right)
			rect.right = rect.left + size.cx;		
		else			
			rect.right = rcParent.right;
		MoveWindow(&rect);	
	}
}


void CListEditCtrlLite::SetItemType(int iItemType,int iItemLength/*=-1*/)
{
    m_iItemType = iItemType;
	if(iItemLength == -1)
	{
		m_iItemLength = iItemType;
	}
	else
	{
		m_iItemLength = iItemLength;
	}

    
}