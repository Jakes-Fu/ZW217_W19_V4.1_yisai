#if !defined(AFX_FINDLISTVIEW_H__9BD7D2B3_50B6_4066_8636_0A37C4AC6738__INCLUDED_)
#define AFX_FINDLISTVIEW_H__9BD7D2B3_50B6_4066_8636_0A37C4AC6738__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindListView.h : header file
//
#ifndef __AFXCVIEW_H__
#include <afxcview.h>
#endif // __AFXCVIEW_H__


#pragma warning(push,3)
#include <vector>
#pragma warning(pop)
#pragma warning(disable : 4786 4018)
#include <map>
/////////////////////////////////////////////////////////////////////////////
// CFindListView view

class ITPSet;
class CFindDoc;
class CFindListView : public CListView
{
protected:
	CFindListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFindListView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFindListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void FindItems(BOOL bAutoReflash = FALSE);
	
	//{{AFX_MSG(CFindListView)
	afx_msg void OnDestroy();
    afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CFindDoc* m_pDoc;
    ITPSet  * m_pSet;
	
    int m_nSelectIdx;
	
    DWORD   m_dwStartPos; 
    DWORD   m_dwItemCount;
    std::vector<DWORD> m_vecFind;
	std::vector<DWORD> m_vecColorIdx;
	
	ULONG  m_uLastTPCount;
	
	//teana hu 2011.06.01
	BOOL m_arrSigned [MAX_FIND_DATA_NUM];
private:
	void ClearFind();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDLISTVIEW_H__9BD7D2B3_50B6_4066_8636_0A37C4AC6738__INCLUDED_)
