// BmpToArrayDoc.h : interface of the CBmpToArrayDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMPTOARRAYDOC_H__F757D785_089F_4434_9113_1501C44FC80B__INCLUDED_)
#define AFX_BMPTOARRAYDOC_H__F757D785_089F_4434_9113_1501C44FC80B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBmpToArrayDoc : public CDocument
{
protected: // create from serialization only
	CBmpToArrayDoc();
	DECLARE_DYNCREATE(CBmpToArrayDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpToArrayDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBmpToArrayDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBmpToArrayDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPTOARRAYDOC_H__F757D785_089F_4434_9113_1501C44FC80B__INCLUDED_)
