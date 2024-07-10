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
    void Clear();
    void DisplayBuffer(char *pBuffer, int nLength);
    static char *BufferToString(BYTE *pBuffer, int nLength, int nColumns,int *nResultSize);

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
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
    int       m_nTextLength;
    CString   m_strHexBuffer;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXEDIT_H__18DCAF13_B672_46AF_A509_40655D2E0AD5__INCLUDED_)
