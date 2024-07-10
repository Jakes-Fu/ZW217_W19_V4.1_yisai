#if !defined(AFX_HEXEDIT_H__18DCAF13_B672_46AF_A509_40655D2E0AD5__INCLUDED_)
#define AFX_HEXEDIT_H__18DCAF13_B672_46AF_A509_40655D2E0AD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHexEdit window

class CHexEdit : public CEdit
{
// Public Functions
public:
    void DisplayBuffer(BYTE *pBuffer, int nLength);
    static CString BufferToString(BYTE *pBuffer, int nLength, int nColumns);

// Construction
public:
	CHexEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexEdit)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHexEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHexEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
    BOOL IsEditFull();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXEDIT_H__18DCAF13_B672_46AF_A509_40655D2E0AD5__INCLUDED_)
