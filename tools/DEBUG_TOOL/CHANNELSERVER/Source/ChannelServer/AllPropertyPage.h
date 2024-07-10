#if !defined(AFX_ALLPROPERTYPAGE_H__7ED5EA2E_3CCA_4C4D_BD65_2407AA61E105__INCLUDED_)
#define AFX_ALLPROPERTYPAGE_H__7ED5EA2E_3CCA_4C4D_BD65_2407AA61E105__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AllPropertyPage.h : header file
//
#include "ClientInfoSheet.h"
#include "ConfigureSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CAllPropertyPage

class CAllPropertyPage : public CPropertySheet
{
	DECLARE_DYNAMIC(CAllPropertyPage)

// Construction
public:
	CAllPropertyPage(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAllPropertyPage(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAllPropertyPage();

// Attributes
public:
   CClientInfoSheet   m_PageInfo;
   CConfigureSheet    m_PageCfg;

// Operations
public:
	CFont			  m_fntPage; 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllPropertyPage)
	public:
	virtual BOOL OnInitDialog();
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAllPropertyPage();
	void AddAllPages();

	// Generated message map functions
protected:
	virtual void BuildPropPageArray ();
	//{{AFX_MSG(CAllPropertyPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALLPROPERTYPAGE_H__7ED5EA2E_3CCA_4C4D_BD65_2407AA61E105__INCLUDED_)
