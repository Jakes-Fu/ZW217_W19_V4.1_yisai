#if !defined(AFX_CUSTVIEW_H__E9C9FA3C_A296_4A25_B4DC_C7CA98D9E7B5__INCLUDED_)
#define AFX_CUSTVIEW_H__E9C9FA3C_A296_4A25_B4DC_C7CA98D9E7B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustView view
#include "NVEditorView.h"
class CCustView : public CNVEditorView
{
protected:
	CCustView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCustView)

// Attributes
public:

// Operations
public:
	void  SaveCustBaseNV(LPCTSTR lpPrjFile, BOOL bSaveAs = FALSE);
	virtual void UpdatedMsgAllViews( int nMsgID,CObject* pObj = NULL );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCustView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bLoadCustBase;

private:
	void CreateCustTree(CFileParser* pFileParse, BOOL bNeedMinEdior = TRUE);
	void DestoryCustTree();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTVIEW_H__E9C9FA3C_A296_4A25_B4DC_C7CA98D9E7B5__INCLUDED_)
