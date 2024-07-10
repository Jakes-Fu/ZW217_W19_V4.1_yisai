// MySpin.h: interface for the CMySpin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSPIN_H__003E06A7_A6FB_4334_B220_E3098A439ABD__INCLUDED_)
#define AFX_MYSPIN_H__003E06A7_A6FB_4334_B220_E3098A439ABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "InPlaceEdit.h"
#include "UserDefMsg.h"
#define ID_MYSPIN_EDIT       1500
#define ID_MYSPIN_SPIN       1501

typedef struct tagSPINTEXT
{
    CString m_strTitle;
	CString m_strShowValue;
    int m_nDecValue;
} SPINTEXT, FAR* LPSPINTEXT;

// command define (Spin message)


/////////////////////////////////////////////////////////////////////////////
// CMySpin window

class CMySpin : public CWnd
{
// Construction
public:
	CMySpin();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySpin)
	//}}AFX_VIRTUAL

// Implementation
public:
    void SetEditType( CInPlaceEdit::EDIT_TYPE _type ) { m_Edit.SetEditType( _type );    }
	void SetShowNumber(const CString& _strShowNumber );
	void SetTitle(CString _strTitle);
    void SetPosFlag( bool _bFlag ) { m_bSetPosFlag = _bFlag; };	
    void SetSpinRange( int _nLower, int _nUpper );

    void ShowAllWindow(BOOL bShow = SW_SHOW );
    //to let edit been focused
    void SetFocusOnEdit() { m_Edit.SetFocus();  m_Edit.SetSel(0,-1); }
	virtual ~CMySpin();
	
protected:
   
    // implement body
	CInPlaceEdit m_Edit;
	CSpinButtonCtrl m_Spin;

    //title 
	CString m_strTitle;

    // value show in the cell ( with the prefix of title )    
	int     m_nShowNumber;
	
    // the range of spin control
	int m_nSpinEditLower;
	int m_nSpinEditUpper;

    //wheather or not need to reset the pos
	bool m_bSetPosFlag;
	
	//{{AFX_MSG(CMySpin)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnKillfocusMyEdit();
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_MYSPIN_H__003E06A7_A6FB_4334_B220_E3098A439ABD__INCLUDED_)
