#if !defined(AFX_MSGFLOWVIEW_H__D9D349D3_4AF8_48A0_9196_AC2C52EFCB9E__INCLUDED_)
#define AFX_MSGFLOWVIEW_H__D9D349D3_4AF8_48A0_9196_AC2C52EFCB9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgFlowView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgFlowView view

class CMsgFlowView : public CScrollView
{
public:
	CMsgFlowView();     
    virtual ~CMsgFlowView();
protected:	
	DECLARE_DYNCREATE(CMsgFlowView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgFlowView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMsgFlowView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGFLOWVIEW_H__D9D349D3_4AF8_48A0_9196_AC2C52EFCB9E__INCLUDED_)
