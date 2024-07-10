// NVEditorDoc.h : interface of the CNVEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NVEDITORDOC_H__8A3E2015_8263_4BCC_9529_9BEB2495CFAE__INCLUDED_)
#define AFX_NVEDITORDOC_H__8A3E2015_8263_4BCC_9529_9BEB2495CFAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNVEditorDoc : public CDocument
{
protected: // create from serialization only
	CNVEditorDoc();
	DECLARE_DYNCREATE(CNVEditorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNVEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNVEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNVEditorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NVEDITORDOC_H__8A3E2015_8263_4BCC_9529_9BEB2495CFAE__INCLUDED_)
