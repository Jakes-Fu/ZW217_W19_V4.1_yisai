#if !defined(AFX_KEYINFOVIEW_H__58343641_6474_41B1_8A12_4C87B3D49AAC__INCLUDED_)
#define AFX_KEYINFOVIEW_H__58343641_6474_41B1_8A12_4C87B3D49AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyinfoView.h : header file
//
#include "XListCtrl.h"
#include "ConsDiagStc.h"
#include "ChannelImpuleStc.h"
#pragma warning(push,3)
#include <vector>
#pragma warning(pop)
#pragma warning( disable : 4018)
/////////////////////////////////////////////////////////////////////////////
// CKeyinfoView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CKeyinfoView : public CFormView
{
protected:
	CKeyinfoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CKeyinfoView)

// Form Data
public:
	//{{AFX_DATA(CKeyinfoView)
	enum { IDD = IDD_FORMVIEW_KEYINFO };
		// NOTE: the ClassWizard will add data members here
	CXListCtrl m_lstGSM;
	CXListCtrl m_lstTD;
	CConsDiagStc m_stcConsDiag;
	CChannelImpuleStc m_stcChannelImpule;
	CString m_strConstLine;
	CString m_strImpulseLine;
	//}}AFX_DATA

public:
	void Clear();

// Attributes
public:
	std::vector<WORD>m_vecKeyInfoGSM;
	std::vector<WORD>m_vecKeyInfoTD;
	std::vector<LPSTR>m_vecKey;
	std::vector<LPSTR>m_vecItemGSM;
	std::vector<LPSTR>m_vecItemTD;
	WORD m_wAddrCD;
	WORD m_wAddrCIRP;

private:
	CBrush  m_brshBckGrnd;
	int m_nTPCount;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyinfoView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CKeyinfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CKeyinfoView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnConst();
	afx_msg void OnBtnImpulse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYINFOVIEW_H__58343641_6474_41B1_8A12_4C87B3D49AAC__INCLUDED_)
