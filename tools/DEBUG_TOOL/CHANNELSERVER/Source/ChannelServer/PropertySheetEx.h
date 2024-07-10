#if !defined(AFX_PROPERTYSHEETEX_H__900F0D06_4123_46AF_8BD5_394AF58E2DD9__INCLUDED_)
#define AFX_PROPERTYSHEETEX_H__900F0D06_4123_46AF_8BD5_394AF58E2DD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertySheetEx.h : header file
//

#include "ClientInfoSheet.h"
#include "ConfigureSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetEx

class CPropertySheetEx : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetEx)

// Construction
public:
	CPropertySheetEx(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropertySheetEx(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
    CClientInfoSheet m_ClientInfoPage;
	CConfigureSheet  m_ConfigurePage;

// Operations
public:
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertySheetEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropertySheetEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertySheetEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYSHEETEX_H__900F0D06_4123_46AF_8BD5_394AF58E2DD9__INCLUDED_)
