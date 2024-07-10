#if !defined(AFX_CONPANPAGESHEET_H__D182E968_DAEF_47B9_81C0_BB007791DEEA__INCLUDED_)
#define AFX_CONPANPAGESHEET_H__D182E968_DAEF_47B9_81C0_BB007791DEEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConPanPageSheet.h : header file
//
#include "stdafx.h"
#define CONPAN_DATA		WM_USER+1000

typedef CPropertyPage * CPropertyPagePtr;

#define MAX_SIM_NUM  8
/////////////////////////////////////////////////////////////////////////////
// CConPanPropertySheet

class CConPanPropertySheet : public CPropertySheet
{
	//DECLARE_DYNAMIC(CConPanPropertySheet)

// Construction
public:
	CConPanPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CConPanPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

protected:
	CPropertyPage * m_pSysInfoPage;
	CPropertyPage * m_pMsgFlowPage;

	CPropertyPagePtr m_pSSConPage[MAX_SIM_NUM];
	CPropertyPagePtr m_pCallConPage[MAX_SIM_NUM];
	CPropertyPagePtr m_pSmsConPage[MAX_SIM_NUM];
	CPropertyPagePtr m_pCBConPage[MAX_SIM_NUM];

	CString          m_strSSTitle[MAX_SIM_NUM];
	CString          m_strCallTitle[MAX_SIM_NUM];
	CString          m_strSmsTitle[MAX_SIM_NUM];
	CString          m_strCBTitle[MAX_SIM_NUM];
/*    
    // For the second sim card
    CPropertyPage * m_pSSConPage2;
    CPropertyPage * m_pCallConPage2;
    CPropertyPage * m_pSmsConPage2;
    CPropertyPage * m_pCBConPage2;

	// For the third sim card
	CPropertyPage * m_pSSConPage3;
    CPropertyPage * m_pCallConPage3;
    CPropertyPage * m_pSmsConPage3;
    CPropertyPage * m_pCBConPage3;

	// For the Fourth sim card
	CPropertyPage * m_pSSConPage4;
    CPropertyPage * m_pCallConPage4;
    CPropertyPage * m_pSmsConPage4;
    CPropertyPage * m_pCBConPage4;
*/
	// GSensor
	CPropertyPage * m_pGSensorPage;
    // ATest
    CPropertyPage * m_pATestPage;

// Operations
public:
	BOOL CreateConPan(CWnd *pParent);
	void ReleaseConPan();
   	BOOL IsPowerOn(UINT dual_sys);
    BOOL IsServiceAvailable(UINT dual_sys);
	void DealCPMsg(void *pmsg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConPanPropertySheet)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsAutoPage();
	virtual ~CConPanPropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CConPanPropertySheet)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnRcvCPData(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONPANPAGESHEET_H__D182E968_DAEF_47B9_81C0_BB007791DEEA__INCLUDED_)
