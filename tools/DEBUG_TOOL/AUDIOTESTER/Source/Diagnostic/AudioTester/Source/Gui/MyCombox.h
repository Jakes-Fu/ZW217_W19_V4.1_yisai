#if !defined(AFX_MYCOMBOX_H__5AA4B248_3D83_4B1A_A6C1_112BBC76C7F1__INCLUDED_)
#define AFX_MYCOMBOX_H__5AA4B248_3D83_4B1A_A6C1_112BBC76C7F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCombox.h : header file
//
#include "UserDefMsg.h"
#pragma  warning(push,3)
#include <vector>
#pragma  warning(pop)
/////////////////////////////////////////////////////////////////////////////
// CMyCombox window
typedef struct 
{
    int m_nSelectIdx;
}COMBOX_DATA, *LPCOMBOX_DATA ;
class CMyCombox : public CComboBox
{ 
// Construction
public:
	CMyCombox();

// Attributes
public:
    void SetShowStrings(const std::vector<CString>& _vShowString ){ m_vShowStrings = _vShowString;    }
    void ShowTime(int _nSelect =-1 );
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCombox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyCombox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyCombox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
    std::vector<CString>m_vShowStrings;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCOMBOX_H__5AA4B248_3D83_4B1A_A6C1_112BBC76C7F1__INCLUDED_)
