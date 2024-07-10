/*============================================================================*
 *
 *                          Copyright (c) 2002 Spreadtrum
 *                              All Rights Reserved
 *
 *============================================================================*/


/*============================================================================*
 *                          Edit History
 *-----------------------------------------------------------------------------
 * 2002-11-13   Kai Liu     Created
 *============================================================================*/


#if !defined(AFX_TESTPOINTSVIEW_H__B172879C_ECA7_4315_AF60_31AD4FBA72CE__INCLUDED_)
#define AFX_TESTPOINTSVIEW_H__B172879C_ECA7_4315_AF60_31AD4FBA72CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestPointsView.h : header file
//

#include "DspKernelExport.h"
#include "TPAddrDescDlg.h"
#include "MarkSet.h"
#include "DlgFindTP.h"


#include <AfxTempl.h>
#pragma warning(push,3)
#include <vector>
#pragma warning(pop)
#pragma warning( disable : 4018)

class CMainFrame;
class CDspLoggerDoc;

#define NOT_FIND_TP -1
#pragma warning(disable:4244 4018)

#include <afxcview.h>

/////////////////////////////////////////////////////////////////////////////
// CTestPointsView list view

#define CMassListView CListView
class CTestPointsView : public CMassListView
{
protected:
	CTestPointsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTestPointsView)

public:

// Attributes
public:
	ULONG m_lBaseIdx;
	BOOL  m_bHasPCTime;

// Operations
public:
    void	  Clear();
    void	  RefreshList(ULONG nCount);
    ITPSet*   GetTPSet(){ return m_pTPSet; }  

    void	  GotoItem(int nIndex);
    LONGLONG  FindTp(CString& strAddr,CString& strData,BOOL bUp,CString& strDesc);

   
    void	  PrepareForDisp(void) {	m_bUseFilter = FALSE;}     

    /*@ Liu Kai CR4895 2003-9-23 */
    void	  SetAutoScroll(BOOL bAuto){ m_bAutoScroll = bAuto;  } 
	
	LPCTSTR	GetItemMark(int nMarkIndex, int& nItem);
	ULONG	GetMarkCount();
	void	AddMarkItem(int nItem, LPSTR lpMark);
       
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestPointsView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestPointsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTestPointsView)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnAutoScroll();
	afx_msg void OnUpdateAutoScroll(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditAddrDesc();
	afx_msg void OnTpAddrChart();
	afx_msg void OnGotoItem();
	afx_msg void OnMarkItem();
	afx_msg void OnUnmarkItem();
	afx_msg void OnGotoMark();
	afx_msg void OnMarkUnmarkItem();
	afx_msg void OnNextMark();
	afx_msg void OnPreMark();
	afx_msg void OnRemoveAllMarks();
	afx_msg void OnShowAll();
	afx_msg void OnUpdateTpAddrChart(CCmdUI* pCmdUI);
	afx_msg void OnFilter();
	afx_msg void OnUpdateFilter(CCmdUI* pCmdUI);
	afx_msg void OnTpFind();
	afx_msg void OnTpFindDown();
	afx_msg void OnTpFindUp();
	afx_msg void OnUpdateUIRange(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFindTp(CCmdUI* pCmdUI);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditLstProductInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
    afx_msg void OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg LRESULT OnRefreshLst(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

public:
	void Logel_RelatedLog(ULONG ulPCTime);
	
protected:
    void ShowNotFound()
    {
        CString strText;
        strText.LoadString(IDS_NOT_FOUND);
        AfxMessageBox(strText);
    }

    void ShowFound(ULONGLONG nIndex)
    {
        ASSERT(nIndex != NOT_FIND_TP);
        GotoItem(nIndex);
    }

    ULONGLONG SearchItem(ULONGLONG nIndex,int nSubIndex,BOOL bUp,CString& strText);

    /*@ Liu Kai CR4895 2003-9-23 */
    void CheckFilter(DWORD,DWORD,long);
	//teana hu 2009.12.22
	static BOOL CALLBACK GetFindLogelWndCallBack(HWND hwnd,LPARAM lParam);

protected:
	BOOL m_bShowAll;
    // control autoscroll status of the list view
    BOOL m_bAutoScroll;

    CDlgFindTP m_dlgFind;

    // item count per page
    int m_nItemCount;

//    ULONG m_lBaseIdx;

    std::vector<DWORD> m_vecFindIdx;
    BOOL m_bUseFilter;
   
    CMainFrame* m_pFrame;
    CListCtrl * m_pList;
    CDspLoggerDoc* m_pDoc;

    CTPAddrDescDlg m_dlgDesc;

    CMarkSet m_MarkManage;
    
    ITPSet* m_pTPSet;
	
	DWORD m_dwStartTime;

    friend class CFindTpDlg;

private:
	UINT	m_nFindUpTimer;
	UINT	m_nFindDownTimer;
public:
	void	SetAutoFindUpTimer();
	void	KillAutoFindUpTimer();

	void	SetAutoFindDownTimer();
	void	KillAutoFindDownTimer();


	void	SetAutoFindTimer(BOOL bFindUp =TRUE );
	void	KillAutoFindTimer();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPOINTSVIEW_H__B172879C_ECA7_4315_AF60_31AD4FBA72CE__INCLUDED_)
