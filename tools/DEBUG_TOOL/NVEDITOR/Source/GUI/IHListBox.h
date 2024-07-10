#if !defined(AFX_IHLISTBOX_H__D772FC2C_8280_4DFA_B196_28CA8B997AEB__INCLUDED_)
#define AFX_IHLISTBOX_H__D772FC2C_8280_4DFA_B196_28CA8B997AEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IHListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIHListBox window

class CIHListBox : public CListBox
{
// Construction
public:
	CIHListBox();
	int AddString( LPCTSTR lpszItem );
	int InsertString( int nIndex, LPCTSTR lpszItem );
	
	void RefushHorizontalScrollBar( void );

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIHListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIHListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIHListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IHLISTBOX_H__D772FC2C_8280_4DFA_B196_28CA8B997AEB__INCLUDED_)
