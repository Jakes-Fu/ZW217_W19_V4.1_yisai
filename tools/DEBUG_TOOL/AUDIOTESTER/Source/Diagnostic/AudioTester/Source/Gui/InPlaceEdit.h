#if !defined(AFX_INPLACEEDIT_H__85143C05_62C6_4221_94C1_F685F51D5DF6__INCLUDED_)
#define AFX_INPLACEEDIT_H__85143C05_62C6_4221_94C1_F685F51D5DF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InPlaceEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit window


/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit window
class CInPlaceEdit : public CEdit
{ 
// Construction
public:
    enum EDIT_TYPE
    {
        NORMAL_TEXT,
        DECIMAL_NUMBER,
        HEX_NUMBER,
        FLOAT_NUMBER,
        DEFAULT
    };
	CInPlaceEdit( EDIT_TYPE _type = NORMAL_TEXT );

// Attributes
public:
    void SetEditType( EDIT_TYPE _type ) { m_enumType = _type;    }
    EDIT_TYPE GetEditType()const { return m_enumType;    }
    void SetCheckRange(BOOL bCheckRange = FALSE, double dMaxValue=0.0, double dMinValue=0.0 )
    {
        m_bCheckRange = bCheckRange;
        m_dMaxValue = dMaxValue;
        m_dMinValue = dMinValue;
    }
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInPlaceEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:

    EDIT_TYPE m_enumType;
    //To indicate whether ESC key was pressed
    bool m_bESC;   
   
    BOOL m_bCheckRange;
    double m_dMaxValue;
    double m_dMinValue;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACEEDIT_H__85143C05_62C6_4221_94C1_F685F51D5DF6__INCLUDED_)
