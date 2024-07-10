// MySpin.cpp: implementation of the CMySpin class.
//
//////////////////////////////////////////////////////////////////////
/********************************************************************
**  filename: 	 MySpin.cpp 
**	author:		Xue Wei	
**	Date:	8/4/2004   16:23
**	Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.       

**	Description: 
     combination the function of CEdit and CSpinButtonCtrl to let user can 
show it in the list control cell for exsample
*********************************************************************
*********************************************************************
Edit History
**	DATE                   NAME             DESCRIPTION      
**	8/4/2004              Xue Wei              Create.        

*********************************************************************/

#include "stdafx.h"
#include "AudioTester.h"
#include "MySpin.h"
//#include "MathTools.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL Hex2Decimal( const CString& _strHexValue, CString& _strDecValue );


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// CMySpin

CMySpin::CMySpin():m_nSpinEditLower(0),m_nSpinEditUpper(100),m_Edit(CInPlaceEdit::HEX_NUMBER)
{
	m_strTitle.Empty();
	m_nShowNumber = 0;
	
	m_bSetPosFlag = true;
}

CMySpin::~CMySpin()
{
}


BEGIN_MESSAGE_MAP(CMySpin, CWnd)
	//{{AFX_MSG_MAP(CMySpin)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_NOTIFY(UDN_DELTAPOS, ID_MYSPIN_SPIN, OnDeltaposSpin)
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(ID_MYSPIN_EDIT, OnKillfocusMyEdit)
	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMySpin message handlers

int CMySpin::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CFont* font = GetParent()->GetFont();

	CRect rcClient, rcEdit, rcSpin;
	GetClientRect(&rcClient);

	int iSpinWidth = 15;

	// Edit
	DWORD dwEditStyle = WS_CHILD | WS_VISIBLE|ES_UPPERCASE;

	rcEdit.left   = rcClient.left;
	rcEdit.top    = rcClient.top;
	rcEdit.right  = rcEdit.left + (rcClient.Width() - iSpinWidth);
	rcEdit.bottom = rcEdit.top + rcClient.Height();	
	m_Edit.Create(dwEditStyle, rcEdit, this, ID_MYSPIN_EDIT);
	m_Edit.SetFont( font );

	// Spin
	DWORD dwSpinStyle = WS_CHILD | WS_VISIBLE;

	rcSpin.left   = rcEdit.right;
	rcSpin.top    = rcEdit.top;
	rcSpin.right  = rcClient.right;
	rcSpin.bottom = rcEdit.bottom;
	m_Spin.Create(dwSpinStyle, rcSpin, this, ID_MYSPIN_SPIN);
	m_Spin.SetAccel(0, NULL);
	m_Spin.SetRange(m_nSpinEditLower, m_nSpinEditUpper);
    m_Spin.SetBuddy(&m_Edit);
    
	SetTitle(m_strTitle);
    CString strShowStr;
    strShowStr.Format(_T("%d"),m_nShowNumber);
    SetShowNumber( strShowStr );
	
	return 0;
}

void CMySpin::SetSpinRange( int _nLower, int _nUpper )
{
    m_Edit.SetCheckRange(TRUE,_nUpper,_nLower );
	m_nSpinEditLower = _nLower; m_nSpinEditUpper = _nUpper;
	m_Spin.SetRange32( _nLower, _nUpper );	
}
void CMySpin::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rcClient, rcEdit, rcSpin;
	GetClientRect(&rcClient);

	int iSpinWidth = 15;

	// Edit
	rcEdit.left   = rcClient.left;
	rcEdit.top    = rcClient.top;
	rcEdit.right  = rcEdit.left + (rcClient.Width() - iSpinWidth);
	rcEdit.bottom = rcEdit.top + rcClient.Height();	
	m_Edit.MoveWindow(&rcEdit);

	// Spin
	rcSpin.left   = rcEdit.right;
	rcSpin.top    = rcEdit.top;
	rcSpin.right  = rcClient.right;
	rcSpin.bottom = rcEdit.bottom;
	m_Spin.MoveWindow(&rcSpin);
}

void CMySpin::OnKillFocus(CWnd* pNewWnd) 
{

	CWnd::OnKillFocus(pNewWnd);
	if((pNewWnd==&m_Edit)||(pNewWnd==&m_Spin))
		return;
	SetWindowText(_T(""));
	ShowWindow(SW_HIDE);

	//GetOwner()->Invalidate();

	SPINTEXT st;
	st.m_strTitle = m_strTitle;
	st.m_nDecValue = m_nShowNumber;
	CInPlaceEdit::HEX_NUMBER == m_Edit.GetEditType() ? st.m_strShowValue.Format(_T("%X"), m_nShowNumber ):
	                                                    st.m_strShowValue.Format(_T("%d"),m_nShowNumber );
	
	// to let parent window have chance to handle the change
    GetParent()->SendMessage(WM_INPLACE_SPIN, 0, (LPARAM) &st);
	
}

void CMySpin::SetTitle(CString _strTitle)
{
	m_strTitle = _strTitle;

	if (!::IsWindow(m_Edit.GetSafeHwnd()))
		return;

	CString strEdit, strPercent;
	strEdit = m_strTitle;
	strPercent.Format(_T("%d"), m_nShowNumber);
	strEdit += strPercent;
	m_Edit.SetWindowText(strEdit);
}
void CMySpin::SetShowNumber(const CString& _strShowNumber )
{	
	if (::IsWindow(m_Edit.GetSafeHwnd()))
	{
		CString strEdit;
		strEdit = m_strTitle;
        if( CInPlaceEdit::HEX_NUMBER == m_Edit.GetEditType() )
        {
            CString strDecShowNumber;
            Hex2Decimal( _strShowNumber,strDecShowNumber);
#ifdef _UNICODE
			char szTmp[16] = {0};
			WideCharToMultiByte(CP_ACP,0,strDecShowNumber,-1,szTmp,16,NULL,NULL);
			m_nShowNumber = atoi(szTmp);
#else
			m_nShowNumber = atoi((LPSTR)(LPCTSTR)strDecShowNumber);   
#endif         
        }
        else// is decimal number . not think about other type now
        {
#ifdef _UNICODE
			char szTmp[16] = {0};
			WideCharToMultiByte(CP_ACP,0,_strShowNumber,-1,szTmp,16,NULL,NULL);
			m_nShowNumber = atoi(szTmp);
#else
			m_nShowNumber = atoi((LPSTR)(LPCTSTR)_strShowNumber);
#endif
        }      	
		
        strEdit +=_strShowNumber ;
		m_Edit.SetWindowText(strEdit);

        if( m_bSetPosFlag)
            m_Spin.SetPos(m_nShowNumber);
	}	
}

void CMySpin::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iNewData = pNMUpDown->iPos + pNMUpDown->iDelta ;
	if (iNewData >= m_nSpinEditLower && iNewData <= m_nSpinEditUpper )	
	{
		m_bSetPosFlag = false;
        CString strShowNumber;
        CInPlaceEdit::HEX_NUMBER == m_Edit.GetEditType() ? strShowNumber.Format(_T("%X"),iNewData ) :
                                                            strShowNumber.Format(_T("%d"),iNewData );
        SetShowNumber( strShowNumber );
	}	
	*pResult = 0;
}

// sometimes after user enter text into edit and go other place directly,
// CMySpin can not receive a KillFoucs message. so need this function to
// handle this situation
void CMySpin::OnKillfocusMyEdit()
{
    CString strValue;
    int iValue = 0;
    m_Edit.GetWindowText( strValue );
    strValue.Replace( m_strTitle,NULL);
    strValue.TrimLeft();
    strValue.TrimRight();
    if( CInPlaceEdit::HEX_NUMBER == m_Edit.GetEditType() )
    {
        CString sDecValue;
        Hex2Decimal( strValue, sDecValue );
#ifdef _UNICODE
		char szTmp[16] = {0};
		WideCharToMultiByte(CP_ACP,0,sDecValue,-1,szTmp,16,NULL,NULL);
		iValue = atoi(szTmp);
#else
		iValue = atoi( (LPSTR)(LPCTSTR)sDecValue );
#endif
    }
    else// is decimal number . not think about other type now
    {
#ifdef _UNICODE
		char szTmp[16] = {0};
		WideCharToMultiByte(CP_ACP,0,strValue,-1,szTmp,16,NULL,NULL);
		iValue = atoi(szTmp);
#else
		iValue= atoi( (LPSTR)(LPCTSTR)strValue );   
#endif  
    }
   
    
    if( iValue < m_nSpinEditLower || iValue > m_nSpinEditUpper )
    {
        CString sErr;
        sErr.Format(_T(" value is not in the range [%d,%d] "),m_nSpinEditLower,m_nSpinEditUpper);
        AfxMessageBox( sErr );        
        m_Edit.SetFocus();
        m_Edit.SetSel(0,-1);
        return;
    }
    SetWindowText(_T(""));
    ShowAllWindow(SW_HIDE);
//	ShowWindow(SW_HIDE);
    //    m_Edit.ShowWindow( SW_HIDE );
    //    m_Spin.ShowWindow( SW_HIDE );
    
    m_nShowNumber =iValue;	
    
	SPINTEXT st;
    st.m_nDecValue = iValue;
    st.m_strShowValue = strValue;
    st.m_strTitle = m_strTitle;

    // to let parent window have chance to handle the change
	GetParent()->SendMessage(WM_INPLACE_SPIN, 0, (LPARAM) &st);
}
void CMySpin::ShowAllWindow( BOOL bShow /*= SW_SHOW */)
{ 
    ShowWindow(bShow);
    m_Edit.ShowWindow( bShow );
    m_Spin.ShowWindow( bShow );
}