#if !defined(AFX_TESTSHEET_H__50146643_2C91_44D5_9D0A_4D652B9E2C48__INCLUDED_)
#define AFX_TESTSHEET_H__50146643_2C91_44D5_9D0A_4D652B9E2C48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestSheet.h : header file
//

#include "TestPage.h"

/////////////////////////////////////////////////////////////////////////////
// CTestSheet

class CTestSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTestSheet)

// Construction
public:
	CTestSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTestSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

    CTestPage m_TestPage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTestSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSHEET_H__50146643_2C91_44D5_9D0A_4D652B9E2C48__INCLUDED_)
