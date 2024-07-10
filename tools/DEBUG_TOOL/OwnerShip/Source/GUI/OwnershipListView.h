#if !defined(AFX_OWNERSHIPLISTVIEW_H__615F4444_090D_4EC3_9942_E93BC822928C__INCLUDED_)
#define AFX_OWNERSHIPLISTVIEW_H__615F4444_090D_4EC3_9942_E93BC822928C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OwnershipListView.h : header file
//
#include "Ownership.h"
#include "OwnershipDoc.h"


/////////////////////////////////////////////////////////////////////////////
// COwnershipListView view

class COwnershipListView : public CListView
{
protected:
	COwnershipListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COwnershipListView)

// Attributes
public:
	COwnershipDoc* GetDocument();

// Operations
public:
	BOOL LoadCityFile(LPCTSTR lpszFile);
	BOOL LoadNumberFiles(const CStringArray &agFiles);
	BOOL SortAndDisplay();
	BOOL LoadOwnershipFiles(const CStringArray &agFiles);

	BOOL GenerateOwnership(const CStringArray &agFiles);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnershipListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COwnershipListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void ReleaseAllList();

	// Generated message map functions
protected:
	//{{AFX_MSG(COwnershipListView)
	afx_msg void OnLoadTextFiles();
	afx_msg void OnUpdateLoadTextFiles(CCmdUI* pCmdUI);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGenOwnership();
	afx_msg void OnUpdateGenOwnership(CCmdUI* pCmdUI);
	afx_msg void OnLoadMidFiles();
	afx_msg void OnUpdateLoadMidFiles(CCmdUI* pCmdUI);
	afx_msg void OnClear();
	afx_msg void OnUpdateClear(CCmdUI* pCmdUI);
	afx_msg void OnExportText();
	afx_msg void OnUpdateExportText(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	std::vector<NUMBER_INFO_PTR> * m_pListNumberInfo;
	
	CStringArray m_agCities;
	int     m_nUnknownCityID;	

};

#ifndef _DEBUG  // debug version in OwnershipView.cpp
inline COwnershipDoc* COwnershipListView::GetDocument()
{ return (COwnershipDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNERSHIPLISTVIEW_H__615F4444_090D_4EC3_9942_E93BC822928C__INCLUDED_)
