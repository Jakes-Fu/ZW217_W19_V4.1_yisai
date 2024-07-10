// InPlaceEdit.cpp : implementation file
//

#include "stdafx.h"
#include "AudioTester.h"
#include "InPlaceEdit.h"
#include "UserDefMsg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CInPlaceEdit::CInPlaceEdit(EDIT_TYPE _type/*= IPEDIT_TYPE_NORMAL*/)
{
    m_enumType = _type;	
    m_bCheckRange = FALSE;
    m_dMinValue = 0.0;
    m_dMinValue = 0.0;
    
    m_bESC = FALSE;
    m_dMaxValue = 0;
}

CInPlaceEdit::~CInPlaceEdit()
{
}


BEGIN_MESSAGE_MAP(CInPlaceEdit, CEdit)
	//{{AFX_MSG_MAP(CInPlaceEdit)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit message handlers

BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
				|| pMsg->wParam == VK_DELETE
				|| pMsg->wParam == VK_ESCAPE
				|| GetKeyState( VK_CONTROL)
				)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
         return true;   // DO NOT process further
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CInPlaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if( nChar == VK_ESCAPE )
			m_bESC = TRUE;
		GetParent()->SetFocus();
		return;
	}

	if (m_enumType == DECIMAL_NUMBER )
	{
		if (!(nChar >= '0' && nChar <= '9') && nChar !=0x08 && nChar != '-')
			return;        
	}
	else if(m_enumType == HEX_NUMBER )
	{
		if ( !((nChar >= '0' && nChar <= '9') ||
            ( nChar >='a' && nChar <='f')  ||
            ( nChar >= 'A' && nChar <='F') )
            && nChar !=0x08  )
			return;
	}
    else if( m_enumType == FLOAT_NUMBER )
    {
        if (!(nChar >= '0' && nChar <= '9') && nChar !=0x08
            && nChar != '-' && nChar != '.' )
            return;        
    }
 
	CEdit::OnChar(nChar, nRepCnt, nFlags);	
}

int CInPlaceEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;


	// Set the proper font
	CFont* font = GetParent()->GetFont();
	SetFont(font);

	SetFocus();
	SetSel( 0, -1 );
	return 0;
}


void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd) 
{
    if( m_bCheckRange )
    {
        TCHAR sz[16];
        GetWindowText(sz, 16); 
#ifdef _UNICODE
		char szTmp[16] = {0};
		WideCharToMultiByte(CP_ACP,0,sz,-1,szTmp,16,NULL,NULL);
        double dValue = atof(szTmp);
#else
		double dValue = atof(sz);
#endif
        if( dValue > m_dMaxValue || dValue < m_dMinValue )
        {   
			CString strMsg;
			strMsg.Format( _T("Input value is not in the valid range(%f,%f)."),m_dMinValue,m_dMaxValue ); 
            AfxMessageBox( strMsg );       
			dValue = dValue > m_dMaxValue ? m_dMaxValue : dValue;
			dValue = dValue < m_dMinValue ? m_dMinValue : dValue;
			CString strText;
			strText.Format(_T("%.6f"), dValue);
			SetWindowText(strText);
            SetFocus();
            SetSel(0,-1);            
            return;            
        }
    }
    GetParent()->SendMessage(WM_INPLACE_KILL_FOCUS,0,(LPARAM) this );	
	CEdit::OnKillFocus(pNewWnd);
	
    ShowWindow(SW_HIDE);
}

