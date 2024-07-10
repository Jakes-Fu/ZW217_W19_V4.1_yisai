#if !defined(AFX_ENGINEERINGVIEW_H__C1093C17_D72A_454A_9921_F6E4C21C177A__INCLUDED_)
#define AFX_ENGINEERINGVIEW_H__C1093C17_D72A_454A_9921_F6E4C21C177A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EngineeringView.h : header file
//
#include "PrefsStatic.h"
#include "CgdcontPage.h"
#include "SppsratePage.h"
#include "SpfrqPage.h"
#include "SautoattPage.h"
#include "SptestPage.h"
#include "SpidPage.h"
#include "SpuplmnPage.h"
#include "SysconfigPage.h"
#include "CoolFormView.h"
#include "ATCmdPage.h"
#include "DspPage.h"

typedef class _PageInfo		PAGE_INFO;

class _PageInfo
{
public:
	BOOL		bViewClass;			// View flag for runtime checking
	UINT		nID;				// Resource ID for the Page
	CWnd		*pWnd;				// Pointer to the page
	CWnd		*pWndParent;		// Pointer to the parent page if has
	CString		csCaption;			// Caption on the tree
	CString		csParentCaption;	// Caption of the parent on the tree
};

typedef CTypedPtrArray <CPtrArray, PAGE_INFO*>		PAGE_LIST;
typedef CMap<CWnd *, CWnd *, DWORD, DWORD&>			WNDTREE_MAP;

#define CONFIG_PAGE_NUM 9
#define ITEM_HEIGHT 25
/////////////////////////////////////////////////////////////////////////////
// CEngineeringView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CEngineeringView : public CCoolFormView
{
public:
	CEngineeringView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEngineeringView)

// Form Data
public:
	//{{AFX_DATA(CEngineeringView)
	enum { IDD = IDD_FORMVIEW_ENGINEERING };
	CListBox m_lst;
	CPrefsStatic	m_CaptionBarCtrl;
	CStatic			m_PageFrame;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	void SetListText(CString strText);
	CWnd* AddPage(CRuntimeClass *pWndClass, const char *pCaption, UINT nID = 0,CWnd *pDlgParent = NULL);
	CWnd* AddPage(CRuntimeClass *pWndClass, const char *pCaption, UINT nID,const char *pParentCaption);
	void ShowRespondContent(LPCSTR szRet);
	void ClearAll();

private:
	void ShowPage(const PAGE_INFO *pInfo, UINT nShow = SW_SHOW);
	BOOL CreatePage( const PAGE_INFO *pInfo);
	BOOL DestroyPages();
	BOOL RefreshData();

protected:
	PAGE_LIST		m_pInfo;
	CRect			m_FrameRect;	// Rectangle size of a setting page
	CString			m_csLogoText;	// Logo text
	WNDTREE_MAP		m_wndMap;		// MFC CMap class for internal page management
	UINT			m_uOldIndex;

	CCgdcontPage *m_pCgdcontPage;
	CSppsratePage *m_pSppsratePage;
	CSpfrqPage *m_pSpfrqPage;
	CSautoattPage *m_pSautoattPage;
	CSptestPage *m_pSptestPage;
	CSpidPage *m_pSpidPage;
	CSpuplmnPage *m_pSpuplmnPage;
	CSysconfigPage *m_pSysconfigPage;
	CATCmdPage *m_pATCmdPage;
	CDspPage *m_pDspPage;

// Operations
public:

// Overrides
	virtual BOOL CreateWnd(CWnd *pWnd, CCreateContext *pContext = NULL);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEngineeringView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEngineeringView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEngineeringView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeLstItem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENGINEERINGVIEW_H__C1093C17_D72A_454A_9921_F6E4C21C177A__INCLUDED_)
