#if !defined(AFX_SMSVIEW1_H__B43E29A9_8F6A_4366_8C05_0FEA86E65EEE__INCLUDED_)
#define AFX_SMSVIEW1_H__B43E29A9_8F6A_4366_8C05_0FEA86E65EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSView1.h : header file
//
#include "childframewnd.h"
/////////////////////////////////////////////////////////////////////////////
// CSMSView view

class CSMSView : public CView
{
protected:
	CSMSView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSMSView)

// Attributes
public:


public:
	DWORD            m_dwCategID;
	DWORD            m_dwOldCategID;
	CChildFrameWnd * m_pFrameWnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSView)
	public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL
	

// Implementation
protected:
	virtual ~CSMSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSMSView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSVIEW1_H__B43E29A9_8F6A_4366_8C05_0FEA86E65EEE__INCLUDED_)
