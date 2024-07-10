#if !defined(AFX_CONFIGVIEW_H__14C34B28_4B28_4AC7_89E9_A042602ACAE2__INCLUDED_)
#define AFX_CONFIGVIEW_H__14C34B28_4B28_4AC7_89E9_A042602ACAE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigView.h : header file
//
#include "DlgSelectNetwork.h"
#include "DlgDialNetwork.h"
#include "DlgSecurity.h"
#include "DlgOthers.h"
#include "DlgTDGSM.h"
#include "CoolFormView.h"
#include "BaseTabCtrl.h"

#define CFG_PAGE_NUM 5
/////////////////////////////////////////////////////////////////////////////
// CConfigView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CConfigView : public CCoolFormView
{
public:
	CConfigView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CConfigView)

// Form Data
public:
	//{{AFX_DATA(CConfigView)
	enum { IDD = IDD_FORMVIEW_CONFIG };
	CBaseTabCtrl m_tabCtrl;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	void DoTab(int nTab);
	void SetDlgState(CWnd *pWnd,BOOL bShow);
	void ClearAll();

public:
	CDlgSelectNetwork *m_pSelNetDlg;
	CDlgDialNetwork *m_pDialNetDlg;
	CDlgSecurity *m_pSecurityDlg;
	CDlgOthers *m_pOthersDlg;
	CDlgTDGSM *m_pDlgTDGSM;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CConfigView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CConfigView)
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeTabCfg(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGVIEW_H__14C34B28_4B28_4AC7_89E9_A042602ACAE2__INCLUDED_)
