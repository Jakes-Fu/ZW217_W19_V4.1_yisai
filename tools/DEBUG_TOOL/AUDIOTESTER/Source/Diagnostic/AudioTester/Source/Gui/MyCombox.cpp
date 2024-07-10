// MyCombox.cpp : implementation file
//

#include "stdafx.h"
#include "AudioTester.h"
#include "MyCombox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCombox

CMyCombox::CMyCombox()
{
}

CMyCombox::~CMyCombox()
{
}


BEGIN_MESSAGE_MAP(CMyCombox, CComboBox)
	//{{AFX_MSG_MAP(CMyCombox)
	ON_WM_KILLFOCUS()	
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 
/////////////////////////////////////////////////////////////////////////////
// CMyCombox message handlers
void CMyCombox::ShowTime( int _nSelect /*=-1*/)
{        
    ResetContent();
    int nCount = m_vShowStrings.size();
    for( int j=0; j<nCount; ++j )
    {
        int nIdx = AddString( m_vShowStrings[j] );
        SetItemData( nIdx, j );
    }
    if( -1 != _nSelect )
    {
        SetCurSel( _nSelect );
    }

}

void CMyCombox::OnKillFocus(CWnd* pNewWnd) 
{
    ShowWindow(SW_HIDE);   
	CComboBox::OnKillFocus(pNewWnd);	
    
	COMBOX_DATA data;
    data.m_nSelectIdx = GetCurSel();

	if( -1 != data.m_nSelectIdx )
	{
		// to let parent window have chance to handle the change
		GetParent()->SendMessage(WM_INPLACE_COMBOX, 0, (LPARAM) &data);	
	}
}

void CMyCombox::OnSelchange() 
{
    //force combox to lose focus 
	ShowWindow(SW_HIDE);   
    
/*
	COMBOX_DATA data;
    data.m_nSelectIdx = GetCurSel();

    // to let parent window have chance to handle the change
	GetParent()->SendMessage(WM_INPLACE_COMBOX, 0, (LPARAM) &data);	*/

	
}
