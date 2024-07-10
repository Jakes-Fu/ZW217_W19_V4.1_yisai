#if !defined(AFX_KEYINFODOC_H__439B6D90_AA0F_4598_BE0D_782B1EF5BD53__INCLUDED_)
#define AFX_KEYINFODOC_H__439B6D90_AA0F_4598_BE0D_782B1EF5BD53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyInfoDoc.h : header file
//
#define WM_UPDATE_SETTIMER         (WM_USER+1250)
/////////////////////////////////////////////////////////////////////////////
// CKeyInfoDoc document

class CKeyInfoDoc : public CDocument
{
protected:
	CKeyInfoDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CKeyInfoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyInfoDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKeyInfoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CKeyInfoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYINFODOC_H__439B6D90_AA0F_4598_BE0D_782B1EF5BD53__INCLUDED_)
